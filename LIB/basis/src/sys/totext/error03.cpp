#include <basis/sys/totext.hpp>

namespace totext {

	cstr::aeu wmi_error(HRESULT err)
	{
		return api_error(err, L"WMIUTILS.DLL");
	}

}
