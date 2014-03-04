#include "exception_pvt.hpp"
#include <system/totext.hpp>

namespace Ext {

	///==================================================================================== WmiError
#ifndef NDEBUG
	WmiError::WmiError(HRESULT code, PCSTR file, size_t line, PCSTR func):
		WinError(code, file, line, func) {
	}
#else
	WmiError::WmiError(HRESULT code):
		WinError(code) {
	}
#endif

	WmiError * WmiError::clone() const {
		//	printf(L"WmiError::clone()\n");
		return new WmiError(* this);
	}

	ustring WmiError::type() const {
		return L"WmiError";
	}

	ustring WmiError::what() const {
		return ustring(totext::wmi_error(code()).c_str());
	}


#ifndef NDEBUG
	HRESULT HiddenFunctions::CheckWmiFunc(HRESULT res, PCSTR file, size_t line, PCSTR func) {
		if (res != S_OK)
			throw WmiError(res, file, line, func);
		return res;
	}
#else
	HRESULT HiddenFunctions::CheckWmiFunc(HRESULT res) {
		if (res != S_OK)
			throw WmiError(res);
		return res;
	}
#endif

}
