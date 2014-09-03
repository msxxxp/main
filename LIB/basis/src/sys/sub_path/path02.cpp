#include <basis/sys/path.hpp>
#include <basis/sys/cstr.hpp>

namespace path {

	bool set_work_directory(const wchar_t * path)
	{
		return cstr::is_empty(path) ? false : ::SetCurrentDirectoryW(path);
	}

}
