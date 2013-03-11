#include <libbase/console.hpp>
#include <libbase/memory.hpp>

namespace Base {

	size_t consoleout(PCWSTR in, size_t len, DWORD nStdHandle) {
		DWORD written = 0;
		(len && !::WriteConsoleW(::GetStdHandle(nStdHandle), in, len, &written, nullptr));
		return written;
	}

	int safe_vsnprintf(PWSTR buf, size_t len, PCWSTR format, va_list vl) {
		buf[--len] = 0;
		return ::_vsnwprintf(buf, len, format, vl);
	}

	int stdvprintf(DWORD nStdHandle, PCWSTR format, va_list vl) {
		auto_array<wchar_t> buf(64 * 1024);
		safe_vsnprintf(buf.data(), buf.size(), format, vl);
		return consoleout(buf.data(), Str::length(buf.data()), nStdHandle);
	}

	int printf(PCWSTR format, ...) {
		va_list vl;
		va_start(vl, format);
		int ret = stdvprintf(STD_OUTPUT_HANDLE, format, vl);
		va_end(vl);
		return ret;
	}

}
