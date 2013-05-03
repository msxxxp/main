#include <libbase/std.hpp>
#include <libbase/memory.hpp>
#include <libbase/path.hpp>
#include <libbase/string.hpp>

namespace Base {
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
			return unexpand(ret, Base::lengthof(ret), path) ? ustring(ret) : ustring();
			//	return unx ? Result.PathUnix() : Result;
			//	return ustring();
		}

		ustring compact(PCWSTR path, size_t size)
		{
			auto_array<wchar_t> ret(MAX_PATH_LEN);
			return compact(ret.data(), size, path) ? ustring(ret.data()) : ustring();
		}

	}
}
