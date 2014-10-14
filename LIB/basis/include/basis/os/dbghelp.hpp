#ifndef BASIS_OS_DBGHELP_
#define BASIS_OS_DBGHELP_

#include <basis/sys/linkage.hpp>

#include <dbghelp.h>

namespace os {

	struct Dbghelp_dll: private linkage::DynamicLibrary {
		typedef DWORD64 (WINAPI *FSymGetModuleBase64)(HANDLE hProcess, DWORD64 qwAddr);
		typedef PVOID   (WINAPI *FSymFunctionTableAccess64)(HANDLE hProcess, DWORD64 AddrBase);
		typedef WINBOOL (WINAPI *FStackWalk64)(DWORD, HANDLE, HANDLE, LPSTACKFRAME64, PVOID, PREAD_PROCESS_MEMORY_ROUTINE64, PFUNCTION_TABLE_ACCESS_ROUTINE64, PGET_MODULE_BASE_ROUTINE64, PTRANSLATE_ADDRESS_ROUTINE64);
		typedef WINBOOL (WINAPI *FSymCleanup)(HANDLE hProcess);
		typedef DWORD   (WINAPI *FSymGetOptions)(VOID);
		typedef DWORD   (WINAPI *FSymSetOptions)(DWORD SymOptions);
		typedef WINBOOL (WINAPI *FSymInitializeW)(HANDLE hProcess,PCWSTR UserSearchPath,WINBOOL fInvadeProcess);
		typedef WINBOOL (WINAPI *FSymGetModuleInfoW64)(HANDLE hProcess,DWORD64 qwAddr,PIMAGEHLP_MODULEW64 ModuleInfo);
		typedef WINBOOL (WINAPI *FSymFromAddrW)(HANDLE hProcess,DWORD64 Address,PDWORD64 Displacement,PSYMBOL_INFOW Symbol);
		typedef WINBOOL (WINAPI *FSymGetLineFromAddrW64)(HANDLE hProcess,DWORD64 qwAddr,PDWORD pdwDisplacement,PIMAGEHLP_LINEW64 Line64);

		DEFINE_FUNC(SymGetModuleBase64);
		DEFINE_FUNC(SymFunctionTableAccess64);
		DEFINE_FUNC(StackWalk64);
		DEFINE_FUNC(SymCleanup);
		DEFINE_FUNC(SymGetOptions);
		DEFINE_FUNC(SymSetOptions);
		DEFINE_FUNC(SymInitializeW);
		DEFINE_FUNC(SymGetModuleInfoW64);
		DEFINE_FUNC(SymFromAddrW);
		DEFINE_FUNC(SymGetLineFromAddrW64);

		static Dbghelp_dll & inst();

	private:
		Dbghelp_dll();
	};

}

#endif
