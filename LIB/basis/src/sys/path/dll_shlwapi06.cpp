#include <basis/sys/path.hpp>
#include <basis/os/shlwapi.hpp>

namespace path {

	bool is_mask_match(const wchar_t * path, const wchar_t * mask, DWORD flags)
	{
		return os::shlwapi_dll::inst().PathMatchSpecExW(path, mask, flags) == S_OK;
	}

}
