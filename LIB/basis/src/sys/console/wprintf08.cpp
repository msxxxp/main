#include <basis/sys/console.hpp>
#include <basis/sys/cstr.hpp>
#include <basis/sys/memory.hpp>

namespace {
	const size_t DEFAULT_PRINTF_BUFFER = 16 * 1024;
}

namespace console {

	size_t fvprintf(HANDLE hndl, const wchar_t* format, va_list vl)
	{
		memory::auto_array<wchar_t> buf(DEFAULT_PRINTF_BUFFER);
		while (!safe_vsnprintf(buf.data(), buf.size(), format, vl))
			buf.reserve(buf.size() * 2);
		return fputs(buf.data(), cstr::length(buf.data()), hndl);
	}

}
