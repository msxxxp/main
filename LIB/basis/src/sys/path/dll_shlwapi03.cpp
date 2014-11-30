#include <basis/sys/path.hpp>

namespace path {

	bool expand(wchar_t* dest, size_t length, const wchar_t* path)
	{
		return ::ExpandEnvironmentStringsW(path, dest, length);
	}

}
