#include <basis/sys/path.hpp>

#include <basis/std/string>

namespace path {

	bool set_work_directory(const ustring & path)
	{
		return set_work_directory(path.c_str());
	}

}
