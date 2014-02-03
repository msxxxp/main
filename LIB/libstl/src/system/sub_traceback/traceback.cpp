#include <system/traceback.hpp>
#include <system/string.hpp>
#include <system/totext.hpp>

#include <liblog/logger.hpp>

#include <simstd/algorithm>
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
	PCWSTR name;
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

int init_bfd_ctx(bfd_ctx * bc, PCWSTR image)
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
		LogFatal(L"Failed to init bfd from (%s)\n", image);
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

bfd_ctx * get_bc(bfd_set * set, PCWSTR image)
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
			auto static module = logger::get_module(L"traback");
			return module;
		}
	}

	struct Frame::Data {
		Data(size_t frame);

		bool LoadFromPDB(size_t frame);

		bool LoadFromSymbols(size_t frame);

		bool LoadFromMap(size_t frame);

		size_t addr;
		size_t offset;
		uint64_t module_base;
		size_t line;
		ustring source;
		ustring func;
		ustring module;
		ustring image;
	};

	Frame::Data::Data(size_t frame) :
		addr(0), offset(0), module_base(0), line(0), func(L"?")
	{
		IMAGEHLP_MODULEW64 modinfo;
		memory::zero(modinfo);
		modinfo.SizeOfStruct = sizeof(modinfo) - 8;
		BOOL ret = SymGetModuleInfoW64(GetCurrentProcess(), frame, &modinfo);
		LogErrorIf(ret == FALSE, L"%s\n", Base::ErrAsStr().c_str());
		if (ret != FALSE) {
			module = modinfo.ModuleName;
			module_base = modinfo.BaseOfImage;
			image = modinfo.ImageName;
//			LogNoise(L"ModuleName: %s\n", modinfo.ModuleName);
//			LogNoise(L"ImageName: %s\n", modinfo.ImageName);
//			LogNoise(L"LoadedImageName: %s\n", modinfo.LoadedImageName);
//			LogNoise(L"LoadedPdbName: %s\n", modinfo.LoadedPdbName);
//			LogNoise(L"SymType: %d\n", (int)modinfo.SymType);
		}

		(modinfo.SymType && LoadFromPDB(frame)) || LoadFromSymbols(frame) || LoadFromMap(frame);
	}

	bool Frame::Data::LoadFromPDB(size_t frame)
	{
		LogTrace();
		bool ret = false;

		{
			size_t size = sizeof(SYMBOL_INFOW) + MAX_SYM_NAME * sizeof(wchar_t);
			SYMBOL_INFOW * m_syminfo = (SYMBOL_INFOW*)malloc(size);
			m_syminfo->SizeOfStruct = sizeof(*m_syminfo);
			m_syminfo->MaxNameLen = MAX_SYM_NAME;

			DWORD64 displacement;
			BOOL err = SymFromAddrW(GetCurrentProcess(), frame, &displacement, m_syminfo);
			LogErrorIf(err == FALSE, L"%s\n", Base::ErrAsStr().c_str());
			if (err != FALSE) {
				addr = (size_t)m_syminfo->Address;
				func = m_syminfo->Name;
				offset = (size_t)displacement;
				ret = true;
			}
			free(m_syminfo);
		}

		{
			DWORD dummy;
			IMAGEHLP_LINE64 info;
			info.SizeOfStruct = sizeof(info);
			BOOL err = SymGetLineFromAddr64(GetCurrentProcess(), frame, &dummy, &info);
//			LogErrorIf(err == FALSE, L"%s\n", ErrAsStr().c_str());
			if (err != FALSE) {
				line = info.LineNumber;
				source = String::cp2w(filename_only(info.FileName), CP_ACP);
			}
		}
		return ret;
	}

	bool Frame::Data::LoadFromSymbols(size_t frame)
	{
		addr = frame;
#if defined(__GNUC__)
		LogTrace();
		bfd_set * set = static_cast<bfd_set*>(malloc(sizeof(*set)));
		memset(set, 0, sizeof(*set));

		bfd_ctx * bc = get_bc(set, image.c_str());
		if (bc) {
			const char * file = NULL;
			const char * fun = NULL;

			find(bc, frame, file, fun, line, offset);
			LogDebug(L"file: '%S'\n", file);
			LogDebug(L"func: '%S'\n", fun);
			LogDebug(L"line: %d\n", line);

			if (file)
				source = String::cp2w(filename_only(file, '/'), CP_OEMCP);
			if (fun) {
				char buf[MAX_PATH];
				size_t size = sizeof(buf);
				int st = 0;
				abi::__cxa_demangle(fun, buf, &size, &st);
				func = String::cp2w(st ? fun : buf, CP_OEMCP);
			}
		}
		release_set(set);
#endif
		return false;
	}

	bool Frame::Data::LoadFromMap(size_t /*frame*/)
	{
		return false;
	}

	Frame::~Frame()
	{
		delete m_data;
	}

	Frame::Frame(size_t frame) :
		m_frame(frame),
		m_data(nullptr)
	{
	}

	Frame::Frame(Frame && right) :
		m_frame(),
		m_data(nullptr)
	{
		swap(right);
	}

	Frame & Frame::operator =(Frame && right)
	{
		Frame(simstd::move(right)).swap(*this);
		return *this;
	}

	void Frame::swap(Frame & right)
	{
		using simstd::swap;
		swap(m_frame, right.m_frame);
		swap(m_data, right.m_data);
	}

	const ustring & Frame::source() const
	{
		InitData();
		return m_data->source;
	}

	const ustring & Frame::func() const
	{
		InitData();
		return m_data->func;
	}

	const ustring & Frame::module() const
	{
		InitData();
		return m_data->module;
	}

	size_t Frame::addr() const
	{
		InitData();
		return m_data->addr;
	}

	size_t Frame::offset() const
	{
		InitData();
		return m_data->offset;
	}

	size_t Frame::line() const
	{
		InitData();
		return m_data->line;
	}

	void Frame::InitData() const
	{
		if (!m_data)
			m_data = new Data(m_frame);
	}

	ustring Frame::to_str() const
	{
		wchar_t buf[MAX_PATH];
		if (!source().empty())
			safe_snprintf(buf, lengthof(buf), L"[%s] (%p) %s:0x%Ix {%s:%Iu}", module().c_str(), addr(), func().c_str(), offset(), source().c_str(), line());
		else
			safe_snprintf(buf, lengthof(buf), L"[%s] (%p) %s:0x%Ix", module().c_str(), addr(), func().c_str(), offset());
		return ustring(buf);
	}

	///=============================================================================================
	struct SymbolInit {
		static SymbolInit & inst(PCWSTR path = nullptr)
		{
			static SymbolInit instance(path);
			return instance;
		}

	private:
		~SymbolInit()
		{
			LogErrorIf(!SymCleanup(GetCurrentProcess()), L"%s\n", Base::ErrAsStr().c_str());
		}

		SymbolInit(PCWSTR path)
		{
			SymSetOptions(SymGetOptions() | SYMOPT_FAIL_CRITICAL_ERRORS | SYMOPT_LOAD_LINES);
			LogErrorIf(!SymInitializeW(GetCurrentProcess(), path, TRUE), L"%s\n", Base::ErrAsStr().c_str());

#if defined(__GNUC__)
			bfd_init();
#endif
		}
	};

	///=============================================================================================
	Enum::Enum(PCWSTR path, size_t depth)
	{
		LogNoise(L"path: '%s' depth: %Iu\n", path, depth);
		SymbolInit::inst(path);

		CONTEXT ctx;
		memory::zero(ctx);
		ctx.ContextFlags = CONTEXT_FULL;
		RtlCaptureContext(&ctx);

		/* � ��� � Release-������������ ���������� ��������� ���� /Oy- (�� �������� ��������� �� ������), ����� ����� ������.
		 * http://www.bytetalk.net/2011/06/why-rtlcapturecontext-crashes-on.html
		 */
		STACKFRAME64 sf;
		memset(&sf, 0, sizeof(sf));

		DWORD machine = 0;
#ifdef _AMD64_
		sf.AddrPC.Offset = ctx.Rip;
		sf.AddrPC.Mode = AddrModeFlat;
		sf.AddrFrame.Offset = ctx.Rbp;
		sf.AddrFrame.Mode = AddrModeFlat;
		sf.AddrStack.Offset = ctx.Rsp;
		sf.AddrStack.Mode = AddrModeFlat;
		machine = IMAGE_FILE_MACHINE_AMD64;
#else
		sf.AddrPC.Offset = ctx.Eip;
		sf.AddrPC.Mode = AddrModeFlat;
		sf.AddrFrame.Offset = ctx.Ebp;
		sf.AddrFrame.Mode = AddrModeFlat;
		sf.AddrStack.Offset = ctx.Esp;
		sf.AddrStack.Mode = AddrModeFlat;
		machine = IMAGE_FILE_MACHINE_I386;
#endif

		while (depth-- > 0) {
			BOOL res = StackWalk64(machine, GetCurrentProcess(), GetCurrentThread(), &sf, (void*)&ctx, nullptr, &SymFunctionTableAccess64, &SymGetModuleBase64, nullptr);
#ifdef _AMD64_
			if (!res || sf.AddrReturn.Offset == 0)
				break;
			emplace_back(sf.AddrReturn.Offset);
#else
			if (!res || sf.AddrPC.Offset == 0)
			break;
			emplace_back((size_t)sf.AddrPC.Offset);
#endif
		}
		LogNoise(L"depth: %Iu\n", size());
	}

	void Enum::Print() const
	{
#ifdef ENABLE_LOGGER
		auto module = get_logger_module();
		auto scopeLock(module->lock_scope());
		Logger::set_color_mode(module, true);
		auto savedPrefix = Logger::get_prefix(module);
		Logger::set_prefix(module, Logger::Prefix::Time | Logger::Prefix::Thread);
		LogForce(L"Backtrace: [%Iu]\n", size());
		for (size_t i = 0; i < size(); ++i)
			LogForce(L"[%02Iu] %s\n", size() - (i + 1), operator[](i).to_str().c_str());
		Logger::set_prefix(module, savedPrefix);
#endif
	}

}