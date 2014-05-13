#ifndef LIBSTL_SYSTEM_SSTR_FORMAT_HPP_
#define LIBSTL_SYSTEM_SSTR_FORMAT_HPP_

#include <system/sstr.hpp>

namespace sstr {

	ustring format(const wchar_t * format, ...);

	ustring format(const wchar_t * format, va_list args);

	astring format(const char * format, ...);

	astring format(const char * format, va_list args);

}

#endif
