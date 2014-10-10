
#include <basis/sys/console.hpp>
#include <basis/sys/cstr.hpp>
#include <basis/sys/memory.hpp>

namespace {
	const size_t DEFAULT_PRINTF_BUFFER = 8 * 1024;
}

namespace console {

	size_t vprintf_var(Handle hnd, const wchar_t * format, va_list vl)
	{
		memory::auto_array<wchar_t> buf(DEFAULT_PRINTF_BUFFER);
		while (!safe_vsnprintf(buf.data(), buf.size(), format, vl))
			buf.reserve(buf.size() * 2);
		return puts(buf.data(), cstr::length(buf.data()), hnd);
	}

	size_t vprintf(Handle hnd, const wchar_t * format, va_list vl)
	{
		{
			wchar_t buff[DEFAULT_PRINTF_BUFFER / 2];
			if (safe_vsnprintf(buff, lengthof(buff), format, vl))
				return puts(buff, cstr::length(buff), hnd);
		}
		return vprintf_var(hnd, format, vl);
	}

}
