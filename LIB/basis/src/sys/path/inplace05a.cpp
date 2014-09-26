#include <basis/sys/path.hpp>

#include <basis/simstd/string>

namespace path {

	ustring remove_prefix(const ustring & path, const ustring & pref)
	{
		ustring tmp(path);
		return inplace::remove_prefix(tmp, pref);
	}

}
