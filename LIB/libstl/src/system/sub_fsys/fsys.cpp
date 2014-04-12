#include <system/fsys.hpp>

namespace fsys {

	bool del_nt(const wchar_t * path)
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
