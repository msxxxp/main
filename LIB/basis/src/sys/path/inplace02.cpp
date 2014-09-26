#include <basis/sys/path.hpp>

#include <basis/simstd/string>

namespace path {

	namespace inplace {

		ustring & ensure_prefix(ustring & path)
		{
			return ensure_prefix(path, get_path_prefixw());
		}

	}

}
