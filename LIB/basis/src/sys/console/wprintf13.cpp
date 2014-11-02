#include <basis/sys/console.hpp>

namespace console {

	size_t puts8(const wchar_t* str, size_t len, Handle hnd)
	{
		UNUSED(len);
		return puts8(str, hnd);
	}

}
