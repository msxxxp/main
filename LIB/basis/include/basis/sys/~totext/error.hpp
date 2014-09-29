#ifndef BASIS_SYS_TOTEXT_ERROR_HPP_
#define BASIS_SYS_TOTEXT_ERROR_HPP_

#include <basis/sys/totext.hpp>

#include <basis/simstd/iosfwd>

namespace totext {

	ustring api_error(DWORD err = ::GetLastError(), const wchar_t * lib = nullptr);

	ustring nt_status(ULONG status);

	ustring wmi_error(HRESULT err);

}

#endif
