#include <basis/sys/path.hpp>

#include <basis/std/string>

namespace path {

	ustring canonicalize(const ustring &path)
	{
		return canonicalize(path.c_str());
	}

}
