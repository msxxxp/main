#include <libbase/std.hpp>

namespace Base {
	bool safe_vsnprintf(PWSTR buf, size_t len, PCWSTR format, va_list vl)
	{
		buf[--len] = 0;
		unsigned written = static_cast<unsigned>(::_vsnwprintf(buf, len, format, vl));
		return written <= len;
	}
}
