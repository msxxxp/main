#include <libbase/string.hpp>
#include <libbase/pvt/va_list.hpp>

#include <stdio.h>

namespace Base {

	namespace {
		const size_t default_buffer_size = 4 * 1024;
	}

	namespace String {
		ustring format(PCWSTR fmt, ...)
		{
			Base::Va_list args;
			va_start(args, fmt);
			return format(fmt, args);
		}

		ustring format(PCWSTR fmt, va_list args)
		{
			wchar_t buf[default_buffer_size];
			size_t size = lengthof(buf) - 1;
			buf[size] = L'\0';
			::_vsnwprintf(buf, size, fmt, args);
			return ustring(buf);
		}
	}

}
