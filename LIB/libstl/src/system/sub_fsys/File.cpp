#include <system/fsys.hpp>

namespace fys {
	namespace File {

		inline bool del_simple_nt(PCWSTR path)
		{
			return ::DeleteFileW(path);
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

		bool copy(PCWSTR path, PCWSTR dest)
		{
			return ::CopyFileW(path, dest, true) != 0;
		}

		bool copy_link(PCWSTR path, PCWSTR dest)
		{
			return ::CopyFileExW(path, dest, nullptr, nullptr, nullptr,
			COPY_FILE_ALLOW_DECRYPTED_DESTINATION |
			COPY_FILE_COPY_SYMLINK |
			COPY_FILE_FAIL_IF_EXISTS) != 0;
		}

		bool move(PCWSTR path, PCWSTR dest, DWORD flag)
		{
			return ::MoveFileExW(path, dest, flag);
		}

	}
}
