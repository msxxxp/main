#include <basis/sys/totext.hpp>

namespace totext {

	cstr::aeu nt_status(ULONG status)
	{
		return api_error(status, L"NTDLL.DLL");
	}

}
