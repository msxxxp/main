#include <basis/sys/path.hpp>

#include <basis/std/string>

namespace path {

	ustring make(const wchar_t * path, const wchar_t * name)
	{
		return ensure_end_separator(path) + name;
	}

}
