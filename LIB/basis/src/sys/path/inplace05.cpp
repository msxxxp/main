#include <basis/sys/path.hpp>

#include <basis/simstd/string>

namespace path {

	ustring remove_prefix(const ustring& path)
	{
		ustring tmp(path);
		return inplace::remove_prefix(tmp, get_prefixw());
	}

}
