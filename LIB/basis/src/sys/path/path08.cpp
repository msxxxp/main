#include <basis/sys/path.hpp>

#include <basis/simstd/string>

namespace path {

	extern "C" {
		BOOL WINAPI SHGetSpecialFolderPathW(HWND, wchar_t *, int, BOOL);
	}

	ustring get_special(int csidl, bool create)
	{
		wchar_t buf[MAX_PATH_LEN];
		return SHGetSpecialFolderPathW(nullptr, buf, csidl, create) ? ustring(buf) : ustring();
	}

}
