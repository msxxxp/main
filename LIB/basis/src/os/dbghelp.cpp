#include <basis/os/dbghelp.hpp>

namespace os {

	Dbghelp_dll & Dbghelp_dll::inst()
	{
		static Dbghelp_dll ret;
		return ret;
	}

	Dbghelp_dll::Dbghelp_dll() :
		DynamicLibrary(L"Dbghelp.dll")
	{
		GET_DLL_FUNC(SymGetModuleBase64);
		GET_DLL_FUNC(SymFunctionTableAccess64);
		GET_DLL_FUNC(StackWalk64);
		GET_DLL_FUNC(SymCleanup);
		GET_DLL_FUNC(SymGetOptions);
		GET_DLL_FUNC(SymSetOptions);
		GET_DLL_FUNC(SymInitializeW);
		GET_DLL_FUNC(SymGetModuleInfoW64);
		GET_DLL_FUNC(SymFromAddrW);
		GET_DLL_FUNC(SymGetLineFromAddrW64);
	}

}
