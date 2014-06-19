#ifndef BASIS_SYS_SSTR_CONVERT_HPP_
#define BASIS_SYS_SSTR_CONVERT_HPP_

#include <basis/sys/sstr.hpp>

namespace sstr {

	astring w2cp(const wchar_t * str, UINT cp);

	ustring cp2w(const char * str, UINT cp);

}

#endif
