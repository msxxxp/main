#include <basis/sys/path.hpp>
#include <basis/os/shlwapi.hpp>

namespace path {

	bool canonicalize(wchar_t * dest, const wchar_t * path)
	{
		return os::shlwapi_dll::inst().PathCanonicalizeW(dest, path);
	}

}
