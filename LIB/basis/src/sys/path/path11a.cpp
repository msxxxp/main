#include <basis/sys/path.hpp>

#include <basis/simstd/string>

namespace path {

	ustring secure(const ustring &path)
	{
		return secure(path.c_str());
	}

}
