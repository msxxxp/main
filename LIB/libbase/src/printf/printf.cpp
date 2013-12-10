#include <libbase/std.hpp>
#include <libbase/console.hpp>
#include <libbase/memory.hpp>

namespace Base {
	namespace Console {
		const size_t DEFAULT_PRINTF_BUFFER = 8 * 1024;

		size_t out(Handle hnd, PCSTR str, size_t len)
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

		size_t vprintf(Handle hnd, PCSTR format, va_list vl)
		{
			auto_array<char> buf(DEFAULT_PRINTF_BUFFER);
			while (!safe_vsnprintf(buf.data(), buf.size(), format, vl))
				buf.reserve(buf.size() * sizeof(char));
			return out(hnd, buf.data(), Cstr::length(buf.data()));
		}

		int printf(PCSTR format, ...)
		{
			Base::Va_list vl;
			va_start(vl, format);
			return vprintf(Handle::OUTPUT, format, vl);
		}

	}
}
