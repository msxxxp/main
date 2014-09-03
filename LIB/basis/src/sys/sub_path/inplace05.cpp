#include <basis/sys/path.hpp>

#include <basis/std/string>

namespace path {

	ustring remove_prefix(const ustring & path)
	{
		ustring tmp(path);
		return inplace::remove_prefix(tmp, get_path_prefixw());
	}

}
