#include <basis/sys/path.hpp>

#include <basis/simstd/string>

namespace path {

	ustring validate(const ustring &path)
	{
		return validate(path.c_str());
	}

}
