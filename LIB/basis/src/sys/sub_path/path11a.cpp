#include <basis/sys/path.hpp>

#include <basis/std/string>

namespace path {

	ustring secure(const ustring &path)
	{
		return secure(path.c_str());
	}

}
