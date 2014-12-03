#include "exception_pvt.hpp"

#include <basis/sys/logger.hpp>
#include <basis/sys/totext.hpp>

#include <basis/simstd/string>

namespace exception {

	WinError::WinError() :
		WinError(::GetLastError())
	{
	}

	WinError::WinError(DWORD code) :
		m_code(code)
	{
		LogNoise(L"%s\n", what());
	}

	WinError::WinError(PCSTR file, size_t line, PCSTR func) :
		WinError(::GetLastError())
	{
		this->file = file;
		this->func = func;
		this->line = line;
	}

	WinError::WinError(DWORD code, PCSTR file, size_t line, PCSTR func) :
		WinError(code)
	{
		this->file = file;
		this->func = func;
		this->line = line;
	}

	WinError * WinError::clone() const
	{
		return new WinError(*this);
	}

	const wchar_t* WinError::type() const
	{
		return L"WinError";
	}

	const wchar_t* WinError::what() const
	{
		if (Abstract::what() == nullptr)
			change_what(totext::api_error(code()).c_str());
		return Abstract::what();
	}

	DWORD WinError::code() const
	{
		return m_code;
	}

}
