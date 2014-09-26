#include <basis/sys/path.hpp>

#include <basis/simstd/string>

namespace path {

	ustring get_root(const wchar_t * path) {
		wchar_t ret[MAX_PATH];
		if (::GetVolumePathNameW(path, ret, lengthof(ret)))
			return ustring(ret);
		return ustring(path);
	}

}
