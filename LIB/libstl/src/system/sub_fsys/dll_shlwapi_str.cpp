#include <system/fsys.hpp>
#include <system/memory.hpp>
#include <system/string.hpp>

#include <simstd/string>

namespace fsys {

	namespace Path {

		ustring canonicalize(const wchar_t * path)
		{
			wchar_t ret[MAX_PATH_LEN];
			return canonicalize(ret, path) ? ustring(ret) : ustring();
		}

		ustring unexpand(const wchar_t * path)
		{
			//	bool	unx = IsPathUnix(path);
			//	if (unx)
			//		Result.PathWin();
			wchar_t ret[MAX_PATH_LEN];
			return unexpand(ret, lengthof(ret), path) ? ustring(ret) : ustring();
			//	return unx ? Result.PathUnix() : Result;
			//	return ustring();
		}

		ustring compact(const wchar_t * path, size_t size)
		{
			memory::auto_array<wchar_t> ret(MAX_PATH_LEN);
			return compact(ret.data(), size, path) ? ustring(ret.data()) : ustring();
		}

	}
}
