#include <libbase/std.hpp>
#include <wchar.h>

namespace Base {
	bool safe_vsnprintf(PWSTR buf, size_t len, PCWSTR format, va_list vl)
	{
		buf[--len] = 0;
		return static_cast<unsigned>(::_vsnwprintf(buf, len, format, vl)) <= len;
	}
}
