#include <basis/sys/path.hpp>
#include <basis/os/shlwapi.hpp>

namespace path {

	bool is_relative(const wchar_t * path)
	{
		return os::shlwapi_dll::inst().PathIsRelativeW(path);
	}

}
