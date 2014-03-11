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

	Attributes get_attr_nt(PCWSTR path);

	bool is_file_nt(PCWSTR path);

	bool is_dir_nt(PCWSTR path);

	bool is_link_nt(PCWSTR path);

}

#endif
