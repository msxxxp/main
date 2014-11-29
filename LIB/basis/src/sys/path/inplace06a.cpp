#include <basis/sys/path.hpp>

#include <basis/simstd/string>

namespace path {

	ustring ensure_prefix(const ustring& path, const ustring& pref)
	{
		ustring tmp(path);
		return inplace::ensure_prefix(tmp, pref);
	}

}
