#include <system/console.hpp>
#include <system/memory.hpp>

namespace console {

	const size_t DEFAULT_PRINTF_BUFFER = 8 * 1024;

	size_t putc(Handle hnd, wchar_t ch)
	{
		wchar_t str[] = {ch, WSTR_END_C};
		return puts(hnd, str, lengthof(str));
	}

	size_t puts(Handle hnd, const wchar_t * str, size_t len)
	{
		DWORD written = 0;
		if (len) {
			HANDLE hndl = ::GetStdHandle(static_cast<DWORD>(hnd));
			if (!::WriteConsoleW(hndl, str, len, &written, nullptr)) {
				::WriteFile(hndl, str, len * sizeof(wchar_t), &written, nullptr);
				written /= sizeof(wchar_t);
			}
		}
		return written;
	}

	size_t vprintf(Handle hnd, const wchar_t * format, va_list vl)
	{
		memory::auto_array<wchar_t> buf(DEFAULT_PRINTF_BUFFER);
		while (!safe_vsnprintf(buf.data(), buf.size(), format, vl))
			buf.reserve(buf.size() * sizeof(wchar_t));
		return puts(hnd, buf.data(), Cstr::length(buf.data()));
	}

	int printf(Handle hnd, const wchar_t * format, ...)
	{
		Va_list vl;
		va_start(vl, format);
		return vprintf(hnd, format, vl);
	}

	int printf(const wchar_t * format, ...)
	{
		Va_list vl;
		va_start(vl, format);
		return vprintf(Handle::OUTPUT, format, vl);
	}

}
