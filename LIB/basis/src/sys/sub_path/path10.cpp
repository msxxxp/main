#include <basis/sys/path.hpp>

#include <basis/std/string>

namespace path {

	ustring nice(const wchar_t * path)
	{
		return canonicalize(expand(path));
	}

}
