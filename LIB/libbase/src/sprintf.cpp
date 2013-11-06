#include <libbase/std.hpp>
#include <libbase/pvt/va_list.hpp>
#include <wchar.h>

namespace Base {
	bool safe_vsnprintf(PWSTR buf, size_t len, PCWSTR format, va_list vl)
	{
		buf[--len] = 0;
		return static_cast<unsigned>(::_vsnwprintf(buf, len, format, vl)) <= len;
	}

	bool safe_snprintf(PWSTR buff, size_t len, PCWSTR format, ...)
	{
		Base::Va_list vl;
		va_start(vl, format);
		return safe_vsnprintf(buff, len, format, vl);
	}

}
