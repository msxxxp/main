#include <basis/sys/path.hpp>

#include <basis/std/string>

namespace path {

	ustring extract(const ustring & path)
	{
		return path.substr(0, path.find_last_of(PATH_SEPARATORS));
	}

}
