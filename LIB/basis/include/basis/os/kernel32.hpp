#ifndef BASIS_OS_KERNEL32_
#define BASIS_OS_KERNEL32_

#include <basis/sys/linkage.hpp>

namespace os {

	struct kernel32_dll: private linkage::DynamicLibrary {
		typedef BOOL (WINAPI *FIsWow64Process)(HANDLE hProcess, PBOOL Wow64Process);
		typedef BOOL (WINAPI *FWow64DisableWow64FsRedirection)(PVOID * OldValue);
		typedef BOOL (WINAPI *FWow64RevertWow64FsRedirection)(PVOID OldValue);

		DEFINE_FUNC(IsWow64Process);
		DEFINE_FUNC(Wow64DisableWow64FsRedirection);
		DEFINE_FUNC(Wow64RevertWow64FsRedirection);

		static kernel32_dll & inst();

	private:
		kernel32_dll();
	};

}

#endif
