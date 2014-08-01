#include "exception_pvt.hpp"

#include <basis/sys/totext.hpp>

namespace exception {

#ifdef NDEBUG
	HMailError::HMailError(HRESULT code) :
		WinError(code)
	{
	}
#else
	HMailError::HMailError(HRESULT code, PCSTR file, size_t line, PCSTR func):
		WinError(code, file, line, func)
	{
	}
#endif

	HMailError * HMailError::clone() const
	{
		return new HMailError(*this);
	}

	ustring HMailError::type() const
	{
		return L"HMailError";
	}

	ustring HMailError::what() const
	{
		return ustring(totext::api_error(code(), L"hMailServer.exe").c_str());
	}

#ifndef NDEBUG
	HRESULT HiddenFunctions::CheckHMailErrorFunc(HRESULT err, PCSTR file, size_t line, PCSTR func)
	{
		if (err != ERROR_SUCCESS) {
			throw HMailError(err, file, line, func);
		}
		return err;
	}
#else
	HRESULT HiddenFunctions::CheckHMailErrorFunc(HRESULT err)
	{
		if (err != ERROR_SUCCESS) {
			throw HMailError(err);
		}
		return err;
	}
#endif
}
