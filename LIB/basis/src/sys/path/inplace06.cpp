#include <basis/sys/path.hpp>

#include <basis/simstd/string>

namespace path {

	ustring ensure_prefix(const ustring & path)
	{
		ustring tmp(path);
		return inplace::ensure_prefix(tmp, get_path_prefixw());
	}

}
