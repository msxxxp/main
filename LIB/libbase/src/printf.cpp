#include <libbase/console.hpp>
#include <libbase/memory.hpp>

namespace Base {
	namespace Console {
		const size_t DEFAULT_PRINTF_BUFFER = 8 * 1024;

		size_t out(Handle hnd, PCWSTR str, size_t len)
		{
			DWORD written = 0;
			if (len) {
				if (!::WriteConsoleW(::GetStdHandle((DWORD)hnd), str, len, &written, nullptr)) {
					::WriteFile(::GetStdHandle((DWORD)hnd), str, len * sizeof(wchar_t), &written, nullptr);
					written /= sizeof(wchar_t);
				}
			}
			return written;
		}

		size_t vprintf(Handle hnd, PCWSTR format, va_list vl)
		{
			auto_array<wchar_t> buf(DEFAULT_PRINTF_BUFFER);
			while (!safe_vsnprintf(buf.data(), buf.size(), format, vl))
				buf.reserve(buf.size() * 2);
			return out(hnd, buf.data(), Cstr::length(buf.data()));
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
