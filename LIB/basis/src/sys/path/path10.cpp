#include <basis/sys/path.hpp>

#include <basis/simstd/string>

namespace path {

	ustring nice(const wchar_t * path)
	{
		return canonicalize(expand(path));
	}

}
