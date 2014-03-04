#include <system/console.hpp>
#include <system/cstr.hpp>
#include <system/memory.hpp>

namespace console {

	const size_t DEFAULT_PRINTF_BUFFER = 8 * 1024;

	size_t fputs(const char * str, size_t len, HANDLE hndl)
	{
		DWORD written = 0;
		if (len) {
			if (!::WriteConsoleA(hndl, str, len, &written, nullptr)) {
				::WriteFile(hndl, str, len * sizeof(char), &written, nullptr);
				written /= sizeof(char);
			}
		}
		return written;
	}

	size_t puts(const char * str, HANDLE hndl)
	{
	}

	size_t putc(Handle hnd, char ch)
	{
		char str[] = {ch, ASTR_END_C};
		return puts(hnd, str, lengthof(str));
	}

	size_t puts(Handle hnd, const char * str, size_t len)
	{
		DWORD written = 0;
		if (len) {
			HANDLE hndl = ::GetStdHandle(static_cast<DWORD>(hnd));
			if (!::WriteConsoleA(hndl, str, len, &written, nullptr)) {
				::WriteFile(hndl, str, len * sizeof(char), &written, nullptr);
				written /= sizeof(char);
			}
		}
		return written;
	}

	size_t puts(const char * str, size_t len, Handle hnd)
	{
		return puts(hnd, str, len);
	}

	size_t puts(const char * str, Handle hnd)
	{
		return puts(hnd, str, cstr::length(str));
	}

	size_t vprintf(Handle hnd, const char * format, va_list vl)
	{
		memory::auto_array<char> buf(DEFAULT_PRINTF_BUFFER);
		while (!safe_vsnprintf(buf.data(), buf.size(), format, vl))
			buf.reserve(buf.size() * sizeof(char));
		return puts(hnd, buf.data(), cstr::length(buf.data()));
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
