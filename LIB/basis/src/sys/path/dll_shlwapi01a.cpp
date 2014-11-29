#include <basis/sys/path.hpp>

#include <basis/simstd/string>

namespace path {

	bool is_relative(const ustring& path)
	{
		return is_relative(path.c_str());
	}

}
