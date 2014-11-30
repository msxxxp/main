#include <basis/sys/console.hpp>

namespace console {

	size_t fputs(const wchar_t* str, size_t len, HANDLE hndl)
	{
		DWORD written = 0;
		if (len) {
			if (!::WriteConsoleW(hndl, str, len, &written, nullptr)) {
				::WriteFile(hndl, str, len * sizeof(*str), &written, nullptr);
				written /= sizeof(*str);
			}
		}
		return written;
	}

}
