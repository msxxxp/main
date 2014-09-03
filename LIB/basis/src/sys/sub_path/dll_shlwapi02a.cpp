#include <basis/sys/path.hpp>

#include <basis/std/string>

namespace path {

	ustring canonicalize(const wchar_t * path)
	{
		wchar_t ret[MAX_PATH_LEN];
		return canonicalize(ret, path) ? ustring(ret) : ustring();
	}

}
