#include <basis/sys/path.hpp>
#include <basis/sys/sstr.hpp>

#include <basis/simstd/string>

namespace path {

	ustring secure(const wchar_t * path)
	{
		ustring Result(path);
		sstr::replace_all(Result, ustring(L".."), ustring(L""));
		sstr::replace_all(Result, ustring(L"%"), ustring(L""));
		return validate(Result);
	}

}
