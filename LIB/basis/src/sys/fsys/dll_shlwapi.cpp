#include <basis/sys/path.hpp>
#include <basis/os/shlwapi.hpp>

namespace fsys {

	namespace directory {

		bool is_empty(const wchar_t * path)
		{
			return os::shlwapi_dll::inst().PathIsDirectoryEmptyW(path);
		}

	}

}
