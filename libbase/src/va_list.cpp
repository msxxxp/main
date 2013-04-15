#include <libbase/string.hpp>

#include <stdarg.h>
#include <stdio.h>

namespace Base {

	namespace {
		const size_t default_buffer_size = 4 * 1024;
	}

	namespace String {
		ustring format(PCWSTR fmt, ...) {
			va_list args;
			va_start(args, fmt);
			auto tmp = format(fmt, args);
			va_end(args);
			return tmp;
		}

		ustring format(PCWSTR format, va_list args) {
			wchar_t buf[default_buffer_size];
			size_t size = lengthof(buf) - 1;
			buf[size] = L'\0';
			::_vsnwprintf(buf, size, format, args);
			return ustring(buf);
		}
	}

}
