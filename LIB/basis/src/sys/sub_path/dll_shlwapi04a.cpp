#include <basis/sys/path.hpp>

#include <basis/std/string>

namespace path {

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

}
