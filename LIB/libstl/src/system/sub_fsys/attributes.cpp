#include <system/fsys.hpp>

namespace fsys {

	Attributes get_attr_nt(PCWSTR path)
	{
		return ::GetFileAttributesW(path);
	}

	bool is_file_nt(PCWSTR path)
	{
		Attributes attr = get_attr_nt(path);
		return attr != INVALID_FILE_ATTRIBUTES && is_file(attr);
	}

	bool is_dir_nt(PCWSTR path)
	{
		Attributes attr = get_attr_nt(path);
		return attr != INVALID_FILE_ATTRIBUTES && is_dir(attr);
	}

	bool is_link_nt(PCWSTR path)
	{
		Attributes attr = get_attr_nt(path);
		return attr != INVALID_FILE_ATTRIBUTES && is_link(attr);
	}

}
