#include <basis/sys/fsys.hpp>

namespace fsys {

	Attributes get_attr_nt(const wchar_t * path)
	{
		return ::GetFileAttributesW(path);
	}

	bool is_file_nt(const wchar_t * path)
	{
		Attributes attr = get_attr_nt(path);
		return attr != INVALID_FILE_ATTRIBUTES && is_file(attr);
	}

	bool is_dir_nt(const wchar_t * path)
	{
		Attributes attr = get_attr_nt(path);
		return attr != INVALID_FILE_ATTRIBUTES && is_dir(attr);
	}

	bool is_link_nt(const wchar_t * path)
	{
		Attributes attr = get_attr_nt(path);
		return attr != INVALID_FILE_ATTRIBUTES && is_link(attr);
	}

}
