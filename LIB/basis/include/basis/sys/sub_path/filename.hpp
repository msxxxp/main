#ifndef BASIS_SYS_PATH_FILENAME_HPP_
#define BASIS_SYS_PATH_FILENAME_HPP_

#include <basis/sys/path.hpp>

#include <basis/simstd/iosfwd>

namespace path {

	namespace filename {

		bool is_valid(const wchar_t * name);
		bool is_valid(const ustring & name);

		ustring extract(const ustring & path);

	}

}

#endif
