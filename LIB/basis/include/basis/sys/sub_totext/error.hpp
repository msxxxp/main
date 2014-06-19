#ifndef BASIS_SYS_TOTEXT_ERROR_HPP_
#define BASIS_SYS_TOTEXT_ERROR_HPP_

#include <basis/sys/totext.hpp>

#include <basis/std/iosfwd>

namespace totext {

	simstd::wstring api_error(DWORD err = ::GetLastError(), const wchar_t * lib = nullptr);

	simstd::wstring nt_status(ULONG status);

	simstd::wstring wmi_error(HRESULT err);

}

#endif
