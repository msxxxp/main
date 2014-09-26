#include <basis/sys/fsys.hpp>

namespace fsys {

	namespace directory {

		inline bool del_simple_nt(const wchar_t * path)
		{
			return ::RemoveDirectoryW(path);
		}

		bool del_attrcheck_nt(const wchar_t * path)
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

		bool del_nt(const wchar_t * path)
		{
			return del_simple_nt(path) || del_attrcheck_nt(path);
		}

		bool create_nt(const wchar_t * path, LPSECURITY_ATTRIBUTES lpsa)
		{
			return ::CreateDirectoryW(path, lpsa) || (::GetLastError() == ERROR_ALREADY_EXISTS && fsys::is_dir_nt(path));
		}

		bool copy_link(const wchar_t * path, const wchar_t * dest, LPSECURITY_ATTRIBUTES lpsa)
		{
			return ::CreateDirectoryExW(path, dest, lpsa);
		}
	}

}
