#include <basis/sys/path.hpp>

#include <basis/std/string>

namespace path {

	ustring expand(const ustring & path)
	{
		return expand(path.c_str());
	}

}
