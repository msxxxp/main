#ifndef LIBSTL_SYSTEM_STRING_CONVERT_HPP_
#define LIBSTL_SYSTEM_STRING_CONVERT_HPP_

#include <system/string.hpp>

namespace String {

	astring w2cp(const wchar_t * str, UINT cp);

	ustring cp2w(const char * str, UINT cp);

}

#endif
