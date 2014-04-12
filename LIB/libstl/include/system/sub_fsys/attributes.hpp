#ifndef LIBSTL_SYSTEM_FSYS_ATTRIBUTES_HPP_
#define LIBSTL_SYSTEM_FSYS_ATTRIBUTES_HPP_

#include <system/fsys.hpp>

namespace fsys {

	typedef DWORD Attributes;

	inline bool is_file(Attributes attr)
	{
		return !(attr & FILE_ATTRIBUTE_DIRECTORY);
	}

	inline bool is_dir(Attributes attr)
	{
		return attr & FILE_ATTRIBUTE_DIRECTORY;
	}

	inline bool is_link(Attributes attr)
	{
		return attr & FILE_ATTRIBUTE_REPARSE_POINT;
	}

	Attributes get_attr_nt(const wchar_t * path);

	bool is_file_nt(const wchar_t * path);

	bool is_dir_nt(const wchar_t * path);

	bool is_link_nt(const wchar_t * path);

}

#endif
