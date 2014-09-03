#include <basis/sys/path.hpp>
#include <basis/sys/cstr.hpp>
#include <basis/sys/sstr.hpp>

#include <basis/std/string>

namespace path {

	ustring ensure_end_separator(const ustring & path)
	{
		ustring tmp(path);
		return inplace::ensure_end_separator(tmp, PATH_SEPARATOR_C);
	}

}
