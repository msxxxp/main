#include <basis/sys/console.hpp>

namespace console {

	int printf(Handle hnd, const char* format, ...)
	{
		Va_list vl;
		va_start(vl, format);
		return vprintf(hnd, format, vl);
	}

}
