#include <basis/sys/path.hpp>

#include <basis/std/string>

namespace path {

	namespace inplace {

		ustring & remove_prefix(ustring & path, const ustring & pref)
		{
			if (path.length() >= pref.length() && path.compare(0, pref.length(), pref) == 0)
				path.erase(0, pref.length());
			return path;
		}

	}

}
