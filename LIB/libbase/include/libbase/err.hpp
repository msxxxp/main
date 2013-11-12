#ifndef LIBBASE_ERR_HPP_
#define LIBBASE_ERR_HPP_

#include <libbase/std.hpp>
#include <libbase/wstr.hpp>

namespace Base {

	wstr ErrAsStr(DWORD err = ::GetLastError(), PCWSTR lib = nullptr);

	wstr NTStatusAsStr(ULONG status);

	wstr ErrAsStrWmi(HRESULT err);

}

#endif
