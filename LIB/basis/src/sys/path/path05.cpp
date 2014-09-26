#include <basis/sys/path.hpp>

#include <basis/simstd/string>

namespace path {

	ustring extract_from_mask(const ustring & path)
	{
		ustring tmp(path);
		ustring::size_type pos = tmp.find_first_of(MASK_CHARS);
		if (pos != ustring::npos)
			tmp.erase(pos);
		return extract(tmp);
	}

}
