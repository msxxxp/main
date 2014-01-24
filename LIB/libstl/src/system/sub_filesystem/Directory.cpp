#include <system/fsys.hpp>

namespace fsys {

	namespace Directory {

		inline bool del_simple_nt(PCWSTR path)
		{
			return ::RemoveDirectoryW(path);
		}

		bool del_attrcheck_nt(PCWSTR path)
		{
			DWORD attr = ::GetFileAttributesW(path);
			if (attr != INVALID_FILE_ATTRIBUTES && ::SetFileAttributesW(path, FILE_ATTRIBUTE_NORMAL)) {
				if (del_simple_nt(path))
					return true;
				else
					::SetFileAttributesW(path, attr);
			}
			return false;
		}

		bool del_nt(PCWSTR path)
		{
			return del_simple_nt(path) || del_attrcheck_nt(path);
		}

		bool create_nt(PCWSTR path, LPSECURITY_ATTRIBUTES lpsa)
		{
			return ::CreateDirectoryW(path, lpsa) || (::GetLastError() == ERROR_ALREADY_EXISTS && fsys::is_dir_nt(path));
		}

	}

}
