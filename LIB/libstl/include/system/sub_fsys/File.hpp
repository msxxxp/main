#ifndef LIBSTL_SYSTEM_FSYS_FILE_HPP_
#define LIBSTL_SYSTEM_FSYS_FILE_HPP_

#include <system/fsys.hpp>

namespace fsys {
	namespace File {

		bool del_nt(PCWSTR path);

		inline bool copy(PCWSTR path, PCWSTR dest);

		inline bool copy_link(PCWSTR path, PCWSTR dest);

		inline bool move(PCWSTR path, PCWSTR dest, DWORD flag = 0);

	}
}

#endif
