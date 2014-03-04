#ifndef LIBSTL_SYSTEM_FSYS_DIRECTORY_HPP_
#define LIBSTL_SYSTEM_FSYS_DIRECTORY_HPP_

#include <system/fsys.hpp>

namespace fsys {
	namespace Directory {

		bool del_nt(PCWSTR path);

		bool create_nt(PCWSTR path, LPSECURITY_ATTRIBUTES lpsa = nullptr);

		bool copy_link(PCWSTR path, PCWSTR dest, LPSECURITY_ATTRIBUTES lpsa = nullptr);

	}
}

#endif
