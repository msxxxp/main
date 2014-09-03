#include <basis/sys/path.hpp>

#include <basis/std/string>

namespace path {

	bool is_mask(const ustring & path)
	{
		return is_mask(path.c_str());
	}

}
