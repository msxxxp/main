#ifndef LIBSTL_SYSTEM_TOTEXT_ERROR_HPP_
#define LIBSTL_SYSTEM_TOTEXT_ERROR_HPP_

#include <system/configure.hpp>

#include <simstd/iosfwd>

namespace totext {

	simstd::wstring api_error(DWORD err = ::GetLastError(), const wchar_t * lib = nullptr);

	simstd::wstring nt_status(ULONG status);

	simstd::wstring wmi_error(HRESULT err);

}

#endif
