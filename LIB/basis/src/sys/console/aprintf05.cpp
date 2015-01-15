#include <basis/sys/console.hpp>

namespace console {

	size_t putc(Handle hnd, char ch)
	{
		char str[] = {ch, ASTR_END_C};
		return puts(str, lengthof(str), hnd);
	}

}
