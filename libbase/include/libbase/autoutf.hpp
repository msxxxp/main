#ifndef _LIBBASE_AUTOUTF_HPP_
#define _LIBBASE_AUTOUTF_HPP_

#include <libbase/std.hpp>

#ifdef NoStlString

#else
#include <string>

typedef std::string astring;
typedef std::wstring ustring;
#endif

namespace Base {

	astring w2cp(PCWSTR in, UINT cp);

	ustring cp2w(PCSTR in, UINT cp);

}

#endif
