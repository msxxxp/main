#include <system/fsys.hpp>
#include <system/memory.hpp>
#include <system/string.hpp>

#include <simstd/string>

namespace fsys {

	namespace Path {

		ustring canonicalize(PCWSTR path)
		{
			wchar_t ret[MAX_PATH_LEN];
			return canonicalize(ret, path) ? ustring(ret) : ustring();
		}

		ustring unexpand(PCWSTR path)
		{
			//	bool	unx = IsPathUnix(path);
			//	if (unx)
			//		Result.PathWin();
			wchar_t ret[MAX_PATH_LEN];
			return unexpand(ret, lengthof(ret), path) ? ustring(ret) : ustring();
			//	return unx ? Result.PathUnix() : Result;
			//	return ustring();
		}

		ustring compact(PCWSTR path, size_t size)
		{
			memory::auto_array<wchar_t> ret(MAX_PATH_LEN);
			return compact(ret.data(), size, path) ? ustring(ret.data()) : ustring();
		}

	}
}
