#include <basis/sys/path.hpp>

#include <basis/simstd/string>

namespace path {

	ustring get_root(const ustring & path)
	{
		return get_root(path.c_str());
	}

}
