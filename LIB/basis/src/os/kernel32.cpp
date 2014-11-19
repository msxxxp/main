#include <basis/os/ntdll.hpp>

namespace os {

	ntdll_dll& ntdll_dll::inst()
	{
		static ntdll_dll ret;
		return ret;
	}

	ntdll_dll::ntdll_dll() :
		DynamicLibrary(L"ntdll.dll")
	{
		GET_DLL_FUNC(NtSetInformationThread);
		GET_DLL_FUNC(NtQueryInformationThread);
	}

}
