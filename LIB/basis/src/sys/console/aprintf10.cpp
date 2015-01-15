#include <basis/sys/console.hpp>

namespace console {

	int printf(const char* format, ...)
	{
		Va_list vl;
		va_start(vl, format);
		return vprintf(Handle::OUTPUT, format, vl);
	}

}
