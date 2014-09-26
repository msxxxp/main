#include <basis/sys/path.hpp>

#include <basis/simstd/string>

namespace path {

	ustring compact(const ustring & path, size_t size)
	{
		return compact(path.c_str(), size);
	}

}
