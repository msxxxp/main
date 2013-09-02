#include <libbase/console.hpp>
#include <libbase/memory.hpp>

namespace Base {
	ssize_t safe_vsnprintf(PWSTR buf, size_t len, PCWSTR format, va_list vl)
	{
		buf[--len] = 0;
		return ::_vsnwprintf(buf, len, format, vl);
	}

	namespace Console {
		size_t out(Handle hnd, PCWSTR str, size_t len)
		{
			DWORD written = 0;
			(len && ::WriteConsoleW(::GetStdHandle((DWORD)hnd), str, len, &written, nullptr));
			return written;
		}

		size_t vprintf(Handle hnd, PCWSTR format, va_list vl)
		{
			auto_array<wchar_t> buf(8 * 1024);
			while (safe_vsnprintf(buf.data(), buf.size(), format, vl) < 0)
				buf.reserve(buf.size() * 2);
			return out(hnd, buf.data(), Str::length(buf.data()));
		}

		int printf(PCWSTR format, ...)
		{
			va_list vl;
			va_start(vl, format);
			int ret = vprintf(Handle::OUTPUT, format, vl);
			va_end(vl);
			return ret;
		}

	}
}
