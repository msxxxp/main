#include <basis/sys/path.hpp>
#include <basis/sys/sstr.hpp>

#include <basis/simstd/string>

namespace path {

	ustring validate(const wchar_t * path)
	{
		ustring Result(nice(path));
		sstr::replace_all(Result, ustring(L"..."), ustring(L""));
		sstr::replace_all(Result, ustring(L"\\\\"), ustring(L"\\"));
		if (Result == L"\\")
			Result.clear();
		return Result;
	}

}
