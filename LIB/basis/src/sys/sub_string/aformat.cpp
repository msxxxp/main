#include <basis/sys/sstr.hpp>

#include <basis/std/string>

namespace sstr {

	namespace {
		const size_t default_buffer_size = 4 * 1024;
	}

	astring format(const char * fmt, ...)
	{
		Va_list args;
		va_start(args, fmt);
		return format(fmt, args);
	}

	astring format(const char * fmt, va_list args)
	{
		char buf[default_buffer_size];
		safe_vsnprintf(buf, lengthof(buf), fmt, args);
		return astring(buf);
	}

}
