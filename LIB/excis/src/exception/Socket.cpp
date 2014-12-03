#include "exception_pvt.hpp"

namespace exception {

	WSockError::WSockError() :
		WinError(::WSAGetLastError())
	{
	}

	WSockError::WSockError(DWORD code) :
		WinError(code)
	{
	}

	WSockError::WSockError(PCSTR file, size_t line, PCSTR func) :
		WinError(::WSAGetLastError(), file, line, func)
	{
	}

	WSockError::WSockError(DWORD code, PCSTR file, size_t line, PCSTR func) :
		WinError(code, file, line, func)
	{
	}

	WSockError* WSockError::clone() const
	{
		return new WSockError(*this);
	}

	const wchar_t* WSockError::type() const
	{
		return L"WSockError";
	}

}
