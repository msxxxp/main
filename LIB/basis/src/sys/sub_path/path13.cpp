#include <basis/sys/path.hpp>

#include <basis/std/string>

namespace path {

	ustring temp_dir()
	{
		wchar_t buf[MAX_PATH];
		buf[0] = 0;
		::GetTempPathW(lengthof(buf), buf);
		return ustring(buf);
	}

}
