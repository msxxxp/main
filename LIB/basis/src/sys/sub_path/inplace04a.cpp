#include <basis/sys/path.hpp>
#include <basis/sys/cstr.hpp>
#include <basis/sys/sstr.hpp>

#include <basis/std/string>

namespace path {

	namespace inplace {

		ustring & ensure_end_separator(ustring & path, wchar_t sep)
		{
			if (!path.empty() && !cstr::find(PATH_SEPARATORS, path[path.size() - 1])) {
				path += sep;
			}
			return path;
		}

	}

}
