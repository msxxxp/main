#ifndef _LIBBASE_ERR_HPP_
#define _LIBBASE_ERR_HPP_

#include <libbase/std.hpp>
#include <libbase/sstr.hpp>

namespace Base {

	sstr ErrAsStr(DWORD err = ::GetLastError(), PCWSTR lib = nullptr);

	sstr NTStatusAsStr(ULONG status);

	sstr ErrAsStrWmi(HRESULT err);

}

#endif
