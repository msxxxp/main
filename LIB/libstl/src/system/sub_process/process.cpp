#include <system/process.hpp>

#include <simstd/string>

namespace process {

	ustring get_path()
	{
		wchar_t buf[MAX_PATH_LEN];
		::GetModuleFileNameW(nullptr, buf, lengthof(buf));
		return ustring(buf);
	}

}
