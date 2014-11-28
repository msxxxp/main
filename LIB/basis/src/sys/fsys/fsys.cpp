#include <basis/sys/fsys.hpp>

namespace fsys {

	bool del_nt(const wchar_t* path)
	{
		auto attr = get_attr_nt(path);
		if (is_valid(attr)) {
			if (is_dir(attr)) {
				return directory::del_nt(path);
			} else {
				return file::del_nt(path);
			}
		}
		return false;
	}

	bool is_exist(const wchar_t* path)
	{
		return is_valid(get_attr_nt(path));
	}

}
