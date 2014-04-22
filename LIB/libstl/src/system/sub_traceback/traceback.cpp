#include <system/traceback.hpp>
#include <system/string.hpp>
#include <system/totext.hpp>
#include <extra/pattern.hpp>

#include <liblog/logger.hpp>

#include <simstd/string>

#include <dbghelp.h>

#if defined(__GNUC__)
#include <bfd.h>
#include <cxxabi.h>

struct bfd_ctx {
	bfd * handle;
	asymbol ** symbol;
};

struct bfd_set {
	const wchar_t * name;
	struct bfd_ctx * bc;
	struct bfd_set *next;
};

struct find_info {
	asymbol **symbol;
	bfd_vma counter;
	const char *file;
	const char *func;
	unsigned line;
	size_t displacement;
};

void close_bfd_ctx(bfd_ctx * bc)
{
	if (bc) {
		free(bc->symbol);
		bfd_close(bc->handle);
	}
}

void release_set(bfd_set * set)
{
	while (set) {
		bfd_set * temp = set->next;
		free((void*)set->name);
		close_bfd_ctx(set->bc);
		free(set);
		set = temp;
	}
}

int init_bfd_ctx(bfd_ctx * bc, const wchar_t * image)
{
	bc->handle = NULL;
	bc->symbol = NULL;

	bfd *b = bfd_openr(String::w2cp(image, CP_OEMCP).c_str(), 0);
	if (!b) {
		LogFatal(L"Failed to open bfd from (%s)\n", image);
		return 1;
	}

	int r1 = bfd_check_format(b, bfd_object);
	int r2 = bfd_check_format_matches(b, bfd_object, NULL);
	int r3 = bfd_get_file_flags(b) & HAS_SYMS;

	if (!(r1 && r2 && r3)) {
		bfd_close(b);
		LogAtten(L"Failed to init bfd from (%s)\n", image);
		return 1;
	}

	void * symbol_table;
	unsigned dummy = 0;
	if (bfd_read_minisymbols(b, FALSE, &symbol_table, &dummy) == 0) {
		if (bfd_read_minisymbols(b, TRUE, &symbol_table, &dummy) < 0) {
			free(symbol_table);
			bfd_close(b);
			LogFatal(L"Failed to read symbols from (%s)\n", image);
			return 1;
		}
	}

	bc->handle = b;
	bc->symbol = (asymbol**)symbol_table;

	return 0;
}

bfd_ctx * get_bc(bfd_set * set, const wchar_t * image)
{
	while (set->name) {
		if (wcscmp(set->name, image) == 0)
			return set->bc;
		set = set->next;
	}

	bfd_ctx bc;
	if (init_bfd_ctx(&bc, image)) {
		return NULL;
	}

	set->next = static_cast<bfd_set*>(calloc(1, sizeof(*set)));
	set->bc = static_cast<bfd_ctx*>(malloc(sizeof(bfd_ctx)));
	memcpy(set->bc, &bc, sizeof(bc));
	set->name = wcsdup(image);

	return set->bc;
}

void lookup_section(bfd * abfd, asection * sec, void * opaque_data)
{
	find_info * data = static_cast<find_info*>(opaque_data);

	if (data->func)
		return;

	if (!(bfd_get_section_flags(abfd, sec) & SEC_ALLOC))
		return;

	bfd_vma vma = bfd_get_section_vma(abfd, sec);
	if (data->counter < vma || vma + bfd_get_section_size(sec) <= data->counter)
		return;

	data->displacement = data->counter - vma;
	bfd_find_nearest_line(abfd, sec, data->symbol, data->displacement, &(data->file), &(data->func), &(data->line));
}

void find(bfd_ctx * b, size_t offset, const char *& file, const char *& func, size_t & line, size_t & displacement)
{
	find_info data;
	data.symbol = b->symbol;
	data.counter = offset;
	data.file = NULL;
	data.func = NULL;
	data.line = 0;

	bfd_map_over_sections(b->handle, &lookup_section, &data);

	file = data.file;
	func = data.func;
	line = data.line;
	displacement = 0; //data.displacement;
}
#endif

namespace traceback {

	namespace {
		logger::Module_i * get_logger_module()
		{
			auto static module = logger::get_module(L"traceback");
			return module;
		}
	}

	struct Frame: public Frame_i, private pattern::Uncopyable {
		~Frame();

		Frame(void * address);

		void * address() const override;

		const ustring & module() const override;

		const ustring & file() const override;

		const ustring & function() const override;

		size_t line() const override;

		size_t offset() const override;

		ustring to_str() const override;

	private:
		bool LoadFromPDB(void * frame);

		bool LoadFromSymbols(void * frame);

		bool LoadFromMap(void * frame);

		void * m_address;
		size_t m_offset;
		size_t m_line;
		uint64_t m_module_base;
		ustring m_file;
		ustring m_function;
		ustring m_module;
		ustring m_image;
	};

	Frame::~Frame()
	{
	}

	Frame::Frame(void * address) :
		m_address(0),
		m_offset(0),
		m_line(0),
		m_module_base(0),
		m_function(L"?")
	{
		IMAGEHLP_MODULEW64 modInfo;
		memory::zero(modInfo);
		modInfo.SizeOfStruct = sizeof(modInfo) - 8;

		bool res = ::SymGetModuleInfoW64(::GetCurrentProcess(), reinterpret_cast<DWORD64>(address), &modInfo);
		LogErrorIf(!res, L"%s\n", totext::api_error().c_str());

		if (res) {
			m_module = modInfo.ModuleName;
			m_module_base = modInfo.BaseOfImage;
			m_image = modInfo.ImageName;
			LogNoise(L"BaseOfImage:     %I64u\n", modInfo.BaseOfImage);
			LogNoise(L"ImageSize:       %u\n", modInfo.ImageSize);
			LogNoise(L"TimeDateStamp:   %u\n", modInfo.TimeDateStamp);
			LogNoise(L"CheckSum:        %u\n", modInfo.CheckSum);
			LogNoise(L"NumSyms:         %u\n", modInfo.NumSyms);
			LogNoise(L"SymType:         %d\n", (int)modInfo.SymType);
			LogNoise(L"ModuleName:      '%s'\n", modInfo.ModuleName);
			LogNoise(L"ImageName:       '%s'\n", modInfo.ImageName);
			LogNoise(L"LoadedImageName: '%s'\n", modInfo.LoadedImageName);
			LogNoise(L"LoadedPdbName:   '%s'\n", modInfo.LoadedPdbName);
		}

		(modInfo.SymType && LoadFromPDB(address)) || LoadFromSymbols(address) || LoadFromMap(address);
	}

	void * Frame::address() const
	{
		return m_address;
	}

	const ustring & Frame::module() const
	{
		return m_module;
	}

	const ustring & Frame::file() const
	{
		return m_file;
	}

	const ustring & Frame::function() const
	{
		return m_function;
	}

	size_t Frame::line() const
	{
		return m_line;
	}

	size_t Frame::offset() const
	{
		return m_offset;
	}

	ustring Frame::to_str() const
	{
		return ustring();
	}

	bool Frame::LoadFromPDB(void * address)
	{
		LogTrace();
		bool ret = false;

		{
			size_t size = sizeof(SYMBOL_INFOW) + MAX_SYM_NAME * sizeof(wchar_t);
			SYMBOL_INFOW * symInfo = static_cast<SYMBOL_INFOW*>(malloc(size));
			memory::zero(*symInfo);
			symInfo->SizeOfStruct = sizeof(*symInfo);
			symInfo->MaxNameLen = MAX_SYM_NAME;

			DWORD64 displacement;
			bool res = ::SymFromAddrW(::GetCurrentProcess(), reinterpret_cast<DWORD64>(address), &displacement, symInfo);
			LogErrorIf(!res, L"%s\n", totext::api_error().c_str());
			if (res) {
				m_address = reinterpret_cast<void*>(symInfo->Address);
				m_function = symInfo->Name;
				m_offset = (size_t)displacement;
				ret = true;
			}
			free(symInfo);
		}

		{
			IMAGEHLP_LINEW64 info;
			memory::zero(info);
			info.SizeOfStruct = sizeof(info);

			DWORD dwLineDisplacement = 0;
			bool res = ::SymGetLineFromAddrW64(::GetCurrentProcess(), reinterpret_cast<DWORD64>(address), &dwLineDisplacement, &info);
//			LogErrorIf(!res, L"%s\n", totext::api_error().c_str());
			if (res) {
				m_line = info.LineNumber;
				m_file = info.FileName;
			}
		}
		return ret;
	}

	bool Frame::LoadFromSymbols(void * address)
	{
		LogTrace();
		bool ret = false;
		m_address = address;
#if defined(__GNUC__)
		bfd_set * set = static_cast<bfd_set*>(malloc(sizeof(*set)));
		memory::zero(*set);

		bfd_ctx * bc = get_bc(set, m_image.c_str());
		if (bc) {
			const char * file = NULL;
			const char * func = NULL;

			find(bc, reinterpret_cast<size_t>(m_address), file, func, m_line, m_offset);
			LogNoise(L"file: '%S'\n", file);
			LogNoise(L"func: '%S'\n", func);
			LogNoise(L"line: %Id\n", m_line);
			LogNoise(L"offs: %Id\n", m_offset);

			if (file)
				m_file = String::cp2w(filename_only(file, '/'), CP_OEMCP);
			if (func) {
				char buf[MAX_PATH];
				size_t size = sizeof(buf);
				int st = 0;
				abi::__cxa_demangle(func, buf, &size, &st);
				m_function = String::cp2w(st ? func : buf, CP_OEMCP);
			}
			ret = file || func;
		}
		release_set(set);
#endif
		return ret;
	}

	bool Frame::LoadFromMap(void * address)
	{
		UNUSED(address);
		return false;
	}

	Frame_i * read_frame_data(void * address)
	{
		return new Frame(address);
	}

}
