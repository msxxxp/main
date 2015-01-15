#include <basis/sys/console.hpp>

namespace console {

	int fprintf(HANDLE hndl, const char* format, ...)
	{
		Va_list vl;
		va_start(vl, format);
		return fvprintf(hndl, format, vl);
	}

}
