#include <basis/sys/cstr.hpp>

namespace cstr {

	namespace {
		const size_t default_buffer_size = 4 * 1024;
	}

	aeu format(const wchar_t* fmt, ...)
	{
		Va_list args;
		va_start(args, fmt);
		return format(fmt, args);
	}

	aeu format(const wchar_t* fmt, va_list args)
	{
		wchar_t buf[default_buffer_size];
		auto length = safe_vsnprintf(buf, lengthof(buf), fmt, args);
		return aeu(buf, length);
	}

}
