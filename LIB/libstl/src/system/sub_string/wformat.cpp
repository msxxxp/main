#include <system/string.hpp>
#include <simstl/string>

//#include <stdio.h>

namespace String {

	namespace {
		const size_t default_buffer_size = 4 * 1024;
	}

	simstd::wstring format(const wchar_t * fmt, ...)
	{
		Va_list args;
		va_start(args, fmt);
		return format(fmt, args);
	}

	simstd::wstring format(const wchar_t * fmt, va_list args)
	{
		wchar_t buf[default_buffer_size];
		safe_vsnprintf(buf, lengthof(buf), fmt, args);
		return simstd::wstring(buf);
	}

}
