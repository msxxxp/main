#include <basis/sys/path.hpp>

#include <basis/std/string>

namespace path {

	ustring nice(const ustring &path)
	{
		return nice(path.c_str());
	}

}
