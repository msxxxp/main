#include <libbase/std.hpp>
#include <libbase/filesystem.hpp>

namespace Fsys {
	DWORD get_attr_nt(PCWSTR path)
	{
		return ::GetFileAttributesW(path);
	}

	bool is_dir_nt(PCWSTR path)
	{
		DWORD attr = get_attr_nt(path);
		return attr != INVALID_FILE_ATTRIBUTES && (attr & FILE_ATTRIBUTE_DIRECTORY);
	}

	inline bool del_nt(PCWSTR path)
	{
		DWORD attr = get_attr_nt(path);
		if (attr != INVALID_FILE_ATTRIBUTES ) {
			if (attr & FILE_ATTRIBUTE_DIRECTORY) {
				return Directory::del_nt(path);
			} else {
				return File::del_nt(path);
			}
		}
		return false;
	}
}
