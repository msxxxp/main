#ifndef BASIS_SYS_PATH_INPLACE_HPP_
#define BASIS_SYS_PATH_INPLACE_HPP_

#include <basis/sys/path.hpp>

#include <basis/simstd/iosfwd>

namespace path {

	namespace inplace {

		ustring & remove_prefix(ustring & path);

		ustring & remove_prefix(ustring & path, const ustring & pref);

		ustring & ensure_prefix(ustring & path);

		ustring & ensure_prefix(ustring & path, const ustring & pref);

		ustring & ensure_no_end_separator(ustring & path);

		ustring & ensure_end_separator(ustring & path);

		ustring & ensure_end_separator(ustring & path, wchar_t sep);

	}

	ustring remove_prefix(const ustring & path);

	ustring remove_prefix(const ustring & path, const ustring & pref);

	ustring ensure_prefix(const ustring & path);

	ustring ensure_prefix(const ustring & path, const ustring & pref);

	ustring ensure_no_end_separator(const ustring & path);

	ustring ensure_end_separator(const ustring & path);

	ustring ensure_end_separator(const ustring & path, wchar_t sep);

}

#endif
