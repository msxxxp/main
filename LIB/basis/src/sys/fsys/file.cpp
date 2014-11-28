#include <basis/sys/fsys.hpp>

namespace fsys {

	namespace file {

		inline bool del_simple_nt(const wchar_t* path)
		{
			return ::DeleteFileW(path);
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

		bool copy(const wchar_t* path, const wchar_t* dest)
		{
			return ::CopyFileW(path, dest, true) != 0;
		}

		bool copy_link(const wchar_t* path, const wchar_t* dest)
		{
			return ::CopyFileExW(path, dest, nullptr, nullptr, nullptr,
			COPY_FILE_ALLOW_DECRYPTED_DESTINATION |
			COPY_FILE_COPY_SYMLINK |
			COPY_FILE_FAIL_IF_EXISTS) != 0;
		}

		bool move(const wchar_t* path, const wchar_t* dest, DWORD flag)
		{
			return ::MoveFileExW(path, dest, flag);
		}

	}
}
