#include <basis/sys/console.hpp>
#include <basis/sys/cstr.hpp>

namespace console {

	size_t fputs(const wchar_t * str, HANDLE hndl)
	{
		return fputs(str, cstr::length(str), hndl);
	}

}
