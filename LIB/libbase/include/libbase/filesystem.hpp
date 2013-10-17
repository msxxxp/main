#ifndef _LIBBASE_FILESYSTEM_HPP
#define _LIBBASE_FILESYSTEM_HPP

#include <libbase/std.hpp>

namespace Fsys {
	inline bool is_file(DWORD attr)
	{
		return !(attr & FILE_ATTRIBUTE_DIRECTORY);
	}

	inline bool is_dir(DWORD attr)
	{
		return attr & FILE_ATTRIBUTE_DIRECTORY;
	}

	inline bool is_link(DWORD attr)
	{
		return attr & FILE_ATTRIBUTE_REPARSE_POINT;
	}

	DWORD get_attr_nt(PCWSTR path);

	bool is_file_nt(PCWSTR path);

	bool is_dir_nt(PCWSTR path);

	bool is_link_nt(PCWSTR path);

	bool del_nt(PCWSTR path);

	///======================================================================================== File
	namespace File {
		bool del_nt(PCWSTR path);

		inline bool copy(PCWSTR path, PCWSTR dest) {
			return ::CopyFileW(path, dest, true) != 0;
		}

		inline bool copy_link(PCWSTR path, PCWSTR dest) {
			return ::CopyFileExW(path, dest, nullptr, nullptr, nullptr,
			                     COPY_FILE_ALLOW_DECRYPTED_DESTINATION |
			                     COPY_FILE_COPY_SYMLINK |
			                     COPY_FILE_FAIL_IF_EXISTS) != 0;
		}

		inline bool move(PCWSTR path, PCWSTR dest, DWORD flag = 0) {
			return ::MoveFileExW(path, dest, flag);
		}
	}

	///=================================================================================== Directory
	namespace Directory {
		bool del_nt(PCWSTR path);

		bool create_nt(PCWSTR path, LPSECURITY_ATTRIBUTES lpsa = nullptr);

		inline bool copy_link(PCWSTR path, PCWSTR dest, LPSECURITY_ATTRIBUTES lpsa = nullptr) {
			return ::CreateDirectoryExW(path, dest, lpsa);
		}
	}

	///================================================================================== file_map_t
	struct file_map_t {
		typedef uint64_t size_type;

		~file_map_t();

		file_map_t(PCWSTR path, size_type size = (size_type)-1, bool write = false);

		size_type size() const;

		PVOID data() const;

		bool is_writeble() const;

		bool is_ok() const;

	private:
		uint64_t get_size(HANDLE file) const;

		PVOID m_data;
		size_type m_size;
		bool m_write;
	};

}

#endif
