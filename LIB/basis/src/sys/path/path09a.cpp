#include <basis/sys/path.hpp>

#include <basis/simstd/string>

namespace path {

	ustring get_fullpath(const ustring &path)
	{
		return get_fullpath(path.c_str());
	}

}
