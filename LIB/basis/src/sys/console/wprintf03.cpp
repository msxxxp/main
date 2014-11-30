#include <basis/sys/console.hpp>

namespace console {

	size_t puts(const wchar_t* str, size_t len, Handle hnd)
	{
		return fputs(str, len, ::GetStdHandle(static_cast<DWORD>(hnd)));
	}

}
