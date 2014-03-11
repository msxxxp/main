#include <system/console.hpp>
#include <system/cstr.hpp>
#include <system/memory.hpp>

namespace {
	const size_t DEFAULT_PRINTF_BUFFER = 8 * 1024;
}

namespace console {

	size_t fputs(const char * str, size_t len, HANDLE hndl)
	{
		DWORD written = 0;
		if (len) {
			if (!::WriteConsoleA(hndl, str, len, &written, nullptr)) {
				::WriteFile(hndl, str, len * sizeof(*str), &written, nullptr);
				written /= sizeof(*str);
			}
		}
		return written;
	}

	size_t fputs(const char * str, HANDLE hndl)
	{
		return fputs(str, cstr::length(str), hndl);
	}

	size_t puts(const char * str, size_t len, Handle hnd)
	{
		return fputs(str, len, ::GetStdHandle(static_cast<DWORD>(hnd)));
	}

	size_t puts(const char * str, Handle hnd)
	{
		return fputs(str, ::GetStdHandle(static_cast<DWORD>(hnd)));
	}

	size_t putc(Handle hnd, char ch)
	{
		char str[] = {ch, ASTR_END_C};
		return puts(str, lengthof(str), hnd);
	}

	size_t vprintf(Handle hnd, const char * format, va_list vl)
	{
		memory::auto_array<char> buf(DEFAULT_PRINTF_BUFFER);
		while (!safe_vsnprintf(buf.data(), buf.size(), format, vl))
			buf.reserve(buf.size() * 2);
		return puts(buf.data(), cstr::length(buf.data()), hnd);
	}

	size_t vprintf(const char * format, va_list vl)
	{
		return vprintf(Handle::OUTPUT, format, vl);
	}

	int printf(Handle hnd, const char * format, ...)
	{
		Va_list vl;
		va_start(vl, format);
		return vprintf(hnd, format, vl);
	}

	int printf(const char * format, ...)
	{
		Va_list vl;
		va_start(vl, format);
		return vprintf(Handle::OUTPUT, format, vl);
	}

}
