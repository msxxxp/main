#include <libcom/std.hpp>
#include <libext/exception.hpp>

namespace {

	struct Initializer {
		~Initializer();

		Initializer();
	};

	Initializer::~Initializer()
	{
		::CoUninitialize();
	}

	Initializer::Initializer()
	{
		CheckCom(::CoInitializeEx(nullptr, COINIT_MULTITHREADED));
		CheckCom(::CoInitializeSecurity( nullptr, -1,		// COM negotiates service
		    nullptr,// Authentication services
		    nullptr,// Reserved
		    RPC_C_AUTHN_LEVEL_PKT_PRIVACY,// authentication
		    RPC_C_IMP_LEVEL_IMPERSONATE,// Impersonation
		    nullptr,// Authentication info
		    EOAC_STATIC_CLOAKING,// Additional capabilities
		    nullptr// Reserved
		    ));
	}

}

namespace Com {

	void init()
	{
		static Initializer com;
	}

	HRESULT ConvertErrorToHRESULT(LONG error)
	{
		return (error == ERROR_SUCCESS) ? S_OK : HRESULT_FROM_WIN32(error);
	}

	HRESULT ConvertBoolToHRESULT(bool result)
	{
		return result ? S_OK : ConvertErrorToHRESULT(::GetLastError());
	}
}
