#include <basis/sys/path.hpp>

#include <basis/simstd/string>

namespace path {

	ustring get_fullpath(const wchar_t* path)
	{
		wchar_t buf[MAX_PATH_LEN];
		::GetFullPathNameW(path, lengthof(buf), buf, nullptr);
		return ustring(buf);
	}

}
