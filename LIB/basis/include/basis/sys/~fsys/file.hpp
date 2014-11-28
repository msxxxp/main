#ifndef BASIS_SYS_FSYS_FILE_HPP_
#define BASIS_SYS_FSYS_FILE_HPP_

namespace fsys {

	namespace file {

		bool del_nt(const wchar_t* path);

		bool copy(const wchar_t* path, const wchar_t* dest);

		bool copy_link(const wchar_t* path, const wchar_t* dest);

		bool move(const wchar_t* path, const wchar_t* dest, DWORD flag = 0);

	}

}

#endif
