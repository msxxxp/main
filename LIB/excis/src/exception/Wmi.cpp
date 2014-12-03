#include "exception_pvt.hpp"

#include <basis/sys/totext.hpp>

#include <basis/simstd/string>

namespace exception {

	WmiError::WmiError(HRESULT code) :
		WinError(code)
	{
		change_what(totext::wmi_error(code).c_str());
	}

	WmiError::WmiError(HRESULT code, PCSTR file, size_t line, PCSTR func):
		WinError(code, file, line, func)
	{
		change_what(totext::wmi_error(code).c_str());
	}

	WmiError * WmiError::clone() const
	{
		return new WmiError(*this);
	}

	const wchar_t* WmiError::type() const
	{
		return L"WmiError";
	}

	const wchar_t* WmiError::what() const
	{
		if (Abstract::what() == nullptr)
			change_what(totext::wmi_error(code()).c_str());
		return Abstract::what();
	}

}
