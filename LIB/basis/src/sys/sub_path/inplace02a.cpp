#include <basis/sys/path.hpp>
#include <basis/sys/cstr.hpp>
#include <basis/sys/sstr.hpp>

#include <basis/std/string>

namespace path {

	namespace inplace {

		ustring & ensure_prefix(ustring & path, const ustring & pref)
		{
			if (path.compare(0, 2, NETWORK_PATH_PREFIX) != 0 && path.compare(0, pref.length(), pref) != 0)
				path.insert(0, pref);
			return path;
		}

	}

}
