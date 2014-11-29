#include <basis/sys/path.hpp>

#include <basis/simstd/string>

namespace path {

	namespace inplace {

		ustring& remove_prefix(ustring& path)
		{
			return remove_prefix(path, get_prefixw());
		}

	}

}
