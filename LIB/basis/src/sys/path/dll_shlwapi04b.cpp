#include <basis/sys/path.hpp>

#include <basis/simstd/string>

namespace path {

	ustring unexpand(const ustring& path)
	{
		return unexpand(path.c_str());
	}

}
