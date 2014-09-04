#include <basis/sys/console.hpp>

namespace console {

	size_t putc(Handle hnd, wchar_t ch)
	{
		wchar_t str[] = {ch, WSTR_END_C};
		return puts(str, lengthof(str), hnd);
	}

}
