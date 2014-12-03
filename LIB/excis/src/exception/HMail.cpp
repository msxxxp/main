#include "exception_pvt.hpp"

#include <basis/sys/totext.hpp>

#include <basis/simstd/string>

namespace exception {

	HMailError::HMailError(HRESULT code) :
		WinError(code)
	{
	}

	HMailError::HMailError(HRESULT code, PCSTR file, size_t line, PCSTR func):
		WinError(code, file, line, func)
	{
	}

	HMailError * HMailError::clone() const
	{
		return new HMailError(*this);
	}

	const wchar_t* HMailError::type() const
	{
		return L"HMailError";
	}

	const wchar_t* HMailError::what() const
	{
		if (Abstract::what() == nullptr)
			change_what(totext::api_error(code(), L"hMailServer.exe").c_str());
		return Abstract::what();
	}

}
