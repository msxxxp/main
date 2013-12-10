#include <libbase/std.hpp>
#include <libbase/string.hpp>

#include <stdio.h>

namespace Base {

	namespace {
		const size_t default_buffer_size = 4 * 1024;
	}

	namespace String {
		astring format(PCSTR fmt, ...)
		{
			Base::Va_list args;
			va_start(args, fmt);
			return format(fmt, args);
		}

		astring format(PCSTR fmt, va_list args)
		{
			char buf[default_buffer_size];
			Base::safe_vsnprintf(buf, Base::lengthof(buf), fmt, args);
			return astring(buf);
		}
	}

}
