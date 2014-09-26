#include <basis/sys/path.hpp>
#include <basis/sys/cstr.hpp>
#include <basis/sys/sstr.hpp>

#include <basis/simstd/string>

namespace path {

	ustring ensure_no_end_separator(const ustring & path)
	{
		ustring tmp(path);
		return inplace::ensure_no_end_separator(tmp);
	}

}
