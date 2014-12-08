#include <basis/os/kernel32.hpp>

namespace os {

	kernel32_dll & kernel32_dll::inst()
	{
		static kernel32_dll ret;
		return ret;
	}

	kernel32_dll::kernel32_dll() :
		DynamicLibrary(L"kernel32.dll")
	{
		GET_DLL_FUNC(IsWow64Process);
		GET_DLL_FUNC(Wow64DisableWow64FsRedirection);
		GET_DLL_FUNC(Wow64RevertWow64FsRedirection);
	}

}
