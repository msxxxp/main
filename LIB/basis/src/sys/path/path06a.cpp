#include <basis/sys/path.hpp>

#include <basis/simstd/string>

namespace path {

	ustring make(const ustring& path, const ustring &name)
	{
//		console::printf(L"'%s'\\'%s'", path.c_str(), name.c_str());
		return make(path.c_str(), name.c_str());
	}

}
