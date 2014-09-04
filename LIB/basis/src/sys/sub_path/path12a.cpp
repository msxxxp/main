#include <basis/sys/path.hpp>

#include <basis/std/string>

namespace path {

	ustring validate(const ustring &path)
	{
		return validate(path.c_str());
	}

}
