#include <basis/sys/path.hpp>
#include <basis/sys/cstr.hpp>
#include <basis/sys/sstr.hpp>

#include <basis/simstd/string>

namespace path {

	ustring ensure_end_separator(const ustring& path, wchar_t sep)
	{
		ustring tmp(path);
		return inplace::ensure_end_separator(tmp, sep);
	}

}
