#ifndef LIBSTL_SYSTEM_FSYS_FILE_HPP_
#define LIBSTL_SYSTEM_FSYS_FILE_HPP_

#include <system/fsys.hpp>

namespace fsys {
	namespace File {

		bool del_nt(const wchar_t * path);

		bool copy(const wchar_t * path, const wchar_t * dest);

		bool copy_link(const wchar_t * path, const wchar_t * dest);

		bool move(const wchar_t * path, const wchar_t * dest, DWORD flag = 0);

		struct Map_nt {
			typedef uint64_t size_type;

			~Map_nt();

			Map_nt(const wchar_t * path, size_type size = static_cast<size_type>(-1), bool write = false);

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
}

#endif
