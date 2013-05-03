#ifndef _LIBBASE_FILESYSTEM_HPP
#define _LIBBASE_FILESYSTEM_HPP

#include <libbase/std.hpp>
/*?*/#include <libbase/string.hpp>

namespace Fsys {
	DWORD get_attr_nt(PCWSTR path);

	bool is_dir_nt(PCWSTR path);

	bool del_nt(PCWSTR path);

	namespace File {
		bool del_nt(PCWSTR path);
		inline bool del_nt(const ustring &path) {
			return del_nt(path.c_str());
		}

		inline bool copy(PCWSTR path, PCWSTR dest) {
			return ::CopyFileW(path, dest, true) != 0;
		}
		inline bool copy(const ustring & path, const ustring & dest) {
			return copy(path.c_str(), dest.c_str());
		}

		inline bool move(PCWSTR path, PCWSTR dest, DWORD flag = 0) {
			return ::MoveFileExW(path, dest, flag);
		}
		inline bool move(const ustring & path, const ustring & dest, DWORD flag = 0) {
			return move(path.c_str(), dest.c_str(), flag);
		}
	}

	///=================================================================================== Directory
	namespace Directory {
		bool create_nt(PCWSTR path, LPSECURITY_ATTRIBUTES lpsa = nullptr);

		bool del_nt(PCWSTR path);
	}
}

#endif
