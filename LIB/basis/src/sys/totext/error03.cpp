#include <basis/sys/totext.hpp>

#include <basis/simstd/string>

namespace totext {

	ustring wmi_error(HRESULT err)
	{
		return api_error(err, L"WMIUTILS.DLL");
	}

}
