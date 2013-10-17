#include <libbase/std.hpp>
#include <wchar.h>

namespace Base {
	bool safe_vsnprintf(PWSTR buf, size_t len, PCWSTR format, va_list vl)
	{
		buf[--len] = 0;
		return static_cast<unsigned>(::_vsnwprintf(buf, len, format, vl)) <= len;
	}

	bool safe_snprintf(PWSTR buff, size_t len, PCWSTR format, ...)
	{
		va_list vl;
		va_start(vl, format);
		bool ret = safe_vsnprintf(buff, len, format, vl);
		va_end(vl);
		return ret;
	}

}
