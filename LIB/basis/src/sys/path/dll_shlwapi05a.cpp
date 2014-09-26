#include <basis/sys/path.hpp>
#include <basis/sys/memory.hpp>

#include <basis/simstd/string>

namespace path {

	ustring compact(const wchar_t * path, size_t size)
	{
		memory::auto_array<wchar_t> ret(MAX_PATH_LEN);
		return compact(ret.data(), size, path) ? ustring(ret.data()) : ustring();
	}

}
