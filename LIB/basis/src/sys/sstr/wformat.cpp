#include <basis/sys/sstr.hpp>

#include <basis/simstd/string>

namespace sstr {

	namespace {
		const size_t default_buffer_size = 4 * 1024;
	}

	ustring format(const wchar_t* fmt, ...)
	{
		Va_list args;
		va_start(args, fmt);
		return format(fmt, args);
	}

	ustring format(const wchar_t* fmt, va_list args)
	{
		wchar_t buf[default_buffer_size];
		safe_vsnprintf(buf, lengthof(buf), fmt, args);
		return ustring(buf);
	}

}
