#ifndef BASIS_SYS_TOTEXT_ERROR_HPP_
#define BASIS_SYS_TOTEXT_ERROR_HPP_

namespace totext {

	cstr::aeu api_error(DWORD err = ::GetLastError(), const wchar_t* lib = nullptr);

	cstr::aeu nt_status(ULONG status);

	cstr::aeu wmi_error(HRESULT err);

}

#endif
