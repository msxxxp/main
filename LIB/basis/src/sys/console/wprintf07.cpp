#include <basis/sys/console.hpp>

namespace console {

	size_t vprintf(const wchar_t* format, va_list vl)
	{
		return vprintf(Handle::OUTPUT, format, vl);
	}

}
