#include <basis/sys/totext.hpp>

#include <basis/simstd/string>

namespace totext {

	ustring nt_status(ULONG status)
	{
		return api_error(status, L"NTDLL.DLL");
	}

}
