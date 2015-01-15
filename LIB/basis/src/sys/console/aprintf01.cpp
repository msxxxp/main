#include <basis/sys/console.hpp>

namespace console {

	size_t fputs(const char* str, size_t len, HANDLE hndl)
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

}
