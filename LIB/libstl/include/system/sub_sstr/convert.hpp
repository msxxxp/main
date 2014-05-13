#ifndef LIBSTL_SYSTEM_SSTR_CONVERT_HPP_
#define LIBSTL_SYSTEM_SSTR_CONVERT_HPP_

#include <system/sstr.hpp>

namespace sstr {

	astring w2cp(const wchar_t * str, UINT cp);

	ustring cp2w(const char * str, UINT cp);

}

#endif
