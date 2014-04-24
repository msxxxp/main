#include <liblog/logger.hpp>

#include <system/console.hpp>
#include <system/crt.hpp>

#include <simstd/algorithm>
#include <simstd/vector>

//#include <algorithm>
//#include <vector>
//#include <string>

namespace {
	void setup_logger()
	{
		using namespace logger;
//		console::printf(L"%S:%d\n", __PRETTY_FUNCTION__, __LINE__);
		Default::set_level(Level::Trace);
//		console::printf(L"%S:%d\n", __PRETTY_FUNCTION__, __LINE__);
		Default::set_prefix(Prefix::Full/* | Prefix::Place*/);
//		console::printf(L"%S:%d\n", __PRETTY_FUNCTION__, __LINE__);
	}
}

#ifdef NDEBUG
int wWmain()
#else
int main()
#endif
{
	setup_logger();

	LogTrace();

	LogTrace();

	return 0;
}

/// ========================================================================== Startup (entry point)
#ifdef NDEBUG
extern "C" {
	int atexit(crt::Function pf)
	{
		return crt::atexit(pf);
	}

	void __cxa_pure_virtual(void)
	{
		crt::cxa_pure_virtual();
	}

	int mainCRTStartup()
	{
		//	int	WinMainCRTStartup() {
		crt::init_atexit();
		//		Base::console::printf(L"%S:%d\n", __PRETTY_FUNCTION__, __LINE__);
		int Result = 0;

		//		STARTUPINFO StartupInfo = {sizeof(STARTUPINFO), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
		//		::GetStartupInfo(&StartupInfo);
		//
		//		Result = wWinMain(::GetModuleHandle(nullptr), nullptr, ::GetCommandLine(),
		//						  StartupInfo.dwFlags & STARTF_USESHOWWINDOW ? StartupInfo.wShowWindow : SW_SHOWDEFAULT);
		Result = wWmain();

		crt::invoke_atexit();
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
