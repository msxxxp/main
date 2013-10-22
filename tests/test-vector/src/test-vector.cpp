#include <windows.h>
#include <stdio.h>
#include <libbase/atexit.hpp>

int wWinMain(const wchar_t * pCmdLine);

#ifndef NDEBUG
int wWmain()
#else
int main()
#endif
{
	printf("%s\n", __PRETTY_FUNCTION__);
	return wWinMain(::GetCommandLineW());
}

/// ========================================================================== Startup (entry point)
#ifndef NDEBUG
extern "C" {
	int atexit(Base::FunctionAtExit pf)
	{
		return Base::atexit(pf);
	}

	void __cxa_pure_virtual(void)
	{
		Base::cxa_pure_virtual();
	}

	int	mainCRTStartup() {
//	int	WinMainCRTStartup() {
		Base::init_atexit();
//		Base::Console::printf(L"%S:%d\n", __PRETTY_FUNCTION__, __LINE__);
		int Result = 0;
//		STARTUPINFO StartupInfo = {sizeof(STARTUPINFO), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
//		::GetStartupInfo(&StartupInfo);
//
//		Result = wWinMain(::GetModuleHandle(nullptr), nullptr, ::GetCommandLine(),
//						  StartupInfo.dwFlags & STARTF_USESHOWWINDOW ? StartupInfo.wShowWindow : SW_SHOWDEFAULT);
		Result = wWmain();
		Base::invoke_atexit();
		::ExitProcess(Result);
		return Result;
	}

//	BOOL WINAPI	DllMainCRTStartup(HANDLE, DWORD dwReason, PVOID) {
//		switch (dwReason) {
//			case DLL_PROCESS_ATTACH:
//				init_atexit();
//				break;
//
//			case DLL_PROCESS_DETACH:
//				invoke_atexit();
//				break;
//		}
//		return true;
//	}
}
#endif

