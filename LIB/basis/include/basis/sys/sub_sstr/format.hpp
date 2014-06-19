#ifndef BASIS_SYS_SSTR_FORMAT_HPP_
#define BASIS_SYS_SSTR_FORMAT_HPP_

#include <basis/sys/sstr.hpp>

namespace sstr {

	ustring format(const wchar_t * format, ...);

	ustring format(const wchar_t * format, va_list args);

	astring format(const char * format, ...);

	astring format(const char * format, va_list args);

}

#endif
