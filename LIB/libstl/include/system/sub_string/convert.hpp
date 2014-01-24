#ifndef LIBSTL_SYSTEM_STRING_CONVERT_HPP_
#define LIBSTL_SYSTEM_STRING_CONVERT_HPP_

#include <system/configure.hpp>
#include <simstd/iosfwd>

namespace String {

	simstd::string w2cp(const wchar_t * str, UINT cp);
	simstd::wstring cp2w(const char * str, UINT cp);

}

#endif
