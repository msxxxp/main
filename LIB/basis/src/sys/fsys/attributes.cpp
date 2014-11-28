#include <basis/sys/fsys.hpp>

namespace fsys {

	Attributes get_attr_nt(const wchar_t* path)
	{
		return ::GetFileAttributesW(path);
	}

	bool set_attr_nt(const wchar_t* path, Attributes attr)
	{
		return ::SetFileAttributesW(path, attr);
	}

	bool is_file_nt(const wchar_t* path)
	{
		return is_file(get_attr_nt(path));
	}

	bool is_dir_nt(const wchar_t* path)
	{
		return is_dir(get_attr_nt(path));
	}

	bool is_link_nt(const wchar_t* path)
	{
		return is_link(get_attr_nt(path));
	}

}
