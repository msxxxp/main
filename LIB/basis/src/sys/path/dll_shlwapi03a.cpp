#include <basis/sys/path.hpp>

#include <basis/simstd/string>

namespace path {

	ustring expand(const wchar_t* path)
	{
		wchar_t ret[MAX_PATH_LEN];
		return expand(ret, lengthof(ret), path) ? ustring(ret) : ustring(path);
	}

}
