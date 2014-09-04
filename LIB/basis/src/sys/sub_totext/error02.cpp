#include <basis/sys/totext.hpp>

#include <basis/std/string>

namespace totext {

	ustring nt_status(ULONG status)
	{
		return api_error(status, L"NTDLL.DLL");
	}

}
