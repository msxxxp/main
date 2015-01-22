#include "Bfd.hpp"
#include <basis/sys/cstr.hpp>
#include <basis/sys/logger.hpp>
#include <basis/sys/memory.hpp>
#include <basis/sys/sstr.hpp>

#include <basis/simstd/algorithm>
#include <basis/simstd/string>

#if defined(__GNUC__) && defined(DEBUG) && !defined(_MSC_VER)

namespace {
	struct CompareContextEq {
		CompareContextEq(const wchar_t* image)
			: image(image)
		{
		}

		bool operator ()(const traceback::Bfd::Context& context)
		{
			return cstr::compare_ci(context.image, image) == 0;
		}

		const wchar_t* image;
	};
}

traceback::Bfd::Context::~Context()
{
	LogTraceObjBegin();
	HostFree(memory::heap::DefaultStat, symbol);
	if (handle)
		bfd_close(handle);
	HostFree(memory::heap::DefaultStat, image);
	LogTraceObjEnd();
}

traceback::Bfd::Context::Context(const wchar_t* image)
	: image(cstr::dup(image))
	, handle()
	, symbol()
{
	LogTraceObj();

	bfd* b = bfd_openr(sstr::w2cp(image, CP_OEMCP).c_str(), 0);
	if (!b) {
		LogError(L"Failed to open bfd from (%s)\n", image);
		return;
	}

	auto r1 = bfd_check_format(b, bfd_object);
	auto r2 = bfd_check_format_matches(b, bfd_object, NULL);
	auto r3 = bfd_get_file_flags(b) & HAS_SYMS;

	if (!(r1 && r2 && r3)) {
		bfd_close(b);
		LogError(L"Failed to init bfd from (%s)\n", image);
		return;
	}

	void* symbol_table = nullptr;
	unsigned dummy = 0;
	if (bfd_read_minisymbols(b, FALSE, &symbol_table, &dummy) == 0) {
		if (bfd_read_minisymbols(b, TRUE, &symbol_table, &dummy) < 0) {
			free(symbol_table);
			bfd_close(b);
			LogError(L"Failed to read symbols from (%s)\n", image);
			return;
		}
	}

	handle = b;
	symbol = (asymbol**)symbol_table;
}

traceback::Bfd::Context::Context(Context && other)
	: image(other.image)
	, handle(other.handle)
	, symbol(other.symbol)
{
	LogTraceObj();

	other.image = nullptr;
	other.handle = nullptr;
	other.symbol = nullptr;
}

void traceback::Bfd::Context::find(const char*& file, const char*& func, size_t& line, void* address)
{
	Bfd::Info data;
	data.symbol = symbol;
	data.counter = (size_t)address;
	data.file = nullptr;
	data.func = nullptr;
	data.line = 0;

	bfd_map_over_sections(handle, &lookup_section, &data);

	file = data.file;
	func = data.func;
	line = data.line;
//	displacement = 0; //data.displacement;
}

void traceback::Bfd::Context::lookup_section(bfd* abfd, asection* sec, void* opaque_data)
{
	Bfd::Info* data = static_cast<Bfd::Info*>(opaque_data);

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

traceback::Bfd::~Bfd()
{
	LogTraceObj();
}

traceback::Bfd::Bfd()
{
	LogTraceObj();
}

void traceback::Bfd::fill(const char*& file, const char*& func, size_t& line, const wchar_t* image, void* address)
{
	auto it = simstd::find_if(contxts.begin(), contxts.end(), CompareContextEq(image));
	if (it != contxts.end())
	{
		it->find(file, func, line, address);
	}
	else
	{
		Context ctx(image);
		if (ctx.handle)
		{
			ctx.find(file, func, line, address);
			contxts.emplace_back(simstd::move(ctx));
		}
	}
}

#endif
