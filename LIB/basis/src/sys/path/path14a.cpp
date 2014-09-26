#include <basis/sys/path.hpp>

#include <basis/simstd/string>

namespace path {

	ustring temp_file(const ustring & path)
	{
		return temp_file(path.c_str());
	}

}
