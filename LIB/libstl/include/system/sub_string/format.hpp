#ifndef LIBSTL_SYSTEM_STRING_FORMAT_HPP_
#define LIBSTL_SYSTEM_STRING_FORMAT_HPP_

#include <system/configure.hpp>
#include <simstl/iosfwd>

namespace String {

	simstd::wstring format(const wchar_t * format, ...);
	simstd::wstring format(const wchar_t * format, va_list args);

	simstd::string format(const char * format, ...);
	simstd::string format(const char * format, va_list args);

}

#endif
