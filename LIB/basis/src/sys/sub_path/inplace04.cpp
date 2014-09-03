#include <basis/sys/path.hpp>

#include <basis/std/string>

namespace path {

	namespace inplace {

		ustring & ensure_end_separator(ustring & path)
		{
			return ensure_end_separator(path, PATH_SEPARATOR_C);
		}

	}

}
