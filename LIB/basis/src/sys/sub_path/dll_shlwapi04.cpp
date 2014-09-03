#include <basis/sys/path.hpp>
#include <basis/os/shlwapi.hpp>

namespace path {

	bool unexpand(wchar_t * dest, size_t length, const wchar_t * path)
	{
		return os::shlwapi_dll::inst().PathUnExpandEnvStringsW(path, dest, length);
	}

}
