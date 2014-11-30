#include <basis/sys/fsys.hpp>

namespace fsys {

	namespace directory {

		inline bool del_simple_nt(const wchar_t* path)
		{
			return ::RemoveDirectoryW(path);
		}

		bool del_attrcheck_nt(const wchar_t* path)
		{
			auto attr = get_attr_nt(path);
			if (is_valid(attr) && set_attr_nt(path, FILE_ATTRIBUTE_NORMAL)) {
				if (del_simple_nt(path))
					return true;
				else
					set_attr_nt(path, attr);
			}
			return false;
		}

		bool del_nt(const wchar_t* path)
		{
			return del_simple_nt(path) || del_attrcheck_nt(path);
		}

		bool create_nt(const wchar_t* path, LPSECURITY_ATTRIBUTES lpsa)
		{
			return ::CreateDirectoryW(path, lpsa) || (::GetLastError() == ERROR_ALREADY_EXISTS && fsys::is_dir_nt(path));
		}

		bool copy_link(const wchar_t* path, const wchar_t* dest, LPSECURITY_ATTRIBUTES lpsa)
		{
			return ::CreateDirectoryExW(path, dest, lpsa);
		}
	}

}
