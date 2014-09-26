#include <basis/sys/path.hpp>

#include <basis/simstd/string>

namespace path {

	ustring canonicalize(const ustring &path)
	{
		return canonicalize(path.c_str());
	}

}
