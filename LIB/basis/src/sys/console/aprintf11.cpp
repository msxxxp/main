#include <basis/sys/console.hpp>

namespace console {

	int wprintf(const char* format, ...)
	{
		Va_list vl;
		va_start(vl, format);
		return vprintf(Handle::OUTPUT, format, vl);
	}

}
