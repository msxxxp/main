#ifndef LIBSTL_SYSTEM_FSYS_DIRECTORY_HPP_
#define LIBSTL_SYSTEM_FSYS_DIRECTORY_HPP_

#include <system/fsys.hpp>

namespace fsys {
	namespace Directory {

		bool del_nt(const wchar_t * path);

		bool create_nt(const wchar_t * path, LPSECURITY_ATTRIBUTES lpsa = nullptr);

		bool copy_link(const wchar_t * path, const wchar_t * dest, LPSECURITY_ATTRIBUTES lpsa = nullptr);

	}
}

#endif
