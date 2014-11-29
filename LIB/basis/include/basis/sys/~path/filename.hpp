#ifndef BASIS_SYS_PATH_FILENAME_HPP_
#define BASIS_SYS_PATH_FILENAME_HPP_

namespace path {

	namespace filename {

		bool is_valid(const wchar_t* name);
		bool is_valid(const ustring& name);

		ustring extract(const ustring& path);

	}

}

#endif
