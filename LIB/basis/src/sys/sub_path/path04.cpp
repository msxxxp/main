#include <basis/sys/path.hpp>

#include <basis/std/string>

namespace path {

	bool is_mask(const wchar_t * path)
	{
		ustring tmp(path);
		return tmp.find_first_of(MASK_CHARS) != ustring::npos;
	}

}
