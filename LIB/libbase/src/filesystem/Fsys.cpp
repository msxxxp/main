#include <libbase/std.hpp>
#include <libbase/filesystem.hpp>

namespace Fsys {
	DWORD get_attr_nt(PCWSTR path)
	{
		return ::GetFileAttributesW(path);
	}

	bool is_file_nt(PCWSTR path)
	{
		DWORD attr = get_attr_nt(path);
		return attr != INVALID_FILE_ATTRIBUTES && is_file(attr);
	}

	bool is_dir_nt(PCWSTR path)
	{
		DWORD attr = get_attr_nt(path);
		return attr != INVALID_FILE_ATTRIBUTES && is_dir(attr);
	}

	bool is_link_nt(PCWSTR path)
	{
		DWORD attr = get_attr_nt(path);
		return attr != INVALID_FILE_ATTRIBUTES && is_link(attr);
	}

	bool del_nt(PCWSTR path)
	{
		DWORD attr = get_attr_nt(path);
		if (attr != INVALID_FILE_ATTRIBUTES) {
			if (is_dir(attr)) {
				return Directory::del_nt(path);
			} else {
				return File::del_nt(path);
			}
		}
		return false;
	}
}
