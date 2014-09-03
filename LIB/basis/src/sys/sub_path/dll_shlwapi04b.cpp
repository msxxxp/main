#include <basis/sys/path.hpp>

#include <basis/std/string>

namespace path {

	ustring unexpand(const ustring & path)
	{
		return unexpand(path.c_str());
	}

}
