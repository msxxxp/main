#include <excis/service.hpp>

namespace service {

	ConfigLogonRequest::ConfigLogonRequest():
		serviceStartName(nullptr),
		password(nullptr)
	{
	}

	ConfigLogonRequest::ConfigLogonRequest(const wchar_t* user, const wchar_t* pass):
		serviceStartName(user),
		password(pass)
	{
	}

}
