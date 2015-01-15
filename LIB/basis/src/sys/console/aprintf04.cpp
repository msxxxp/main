#include <basis/sys/console.hpp>

namespace console {

	size_t puts(const char* str, Handle hnd)
	{
		return fputs(str, ::GetStdHandle(static_cast<DWORD>(hnd)));
	}

}
