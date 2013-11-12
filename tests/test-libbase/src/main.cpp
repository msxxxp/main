#include <libbase/std.hpp>
#include <libbase/string.hpp>
#include <libbase/atexit.hpp>
#include <libbase/wstr.hpp>

#include <liblog/logger.hpp>

namespace {
	void setup_logger()
	{
		using namespace Logger;
		Default::set_level(Level::Trace);
		Default::set_prefix(Prefix::Medium | Prefix::Place);
		Default::set_target(get_TargetToConsole());
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
	Base::ustring_test();

	LogTrace();
	Base::mstring::unit_test();

	return 0;
}

/// ========================================================================== Startup (entry point)
#ifdef NDEBUG
extern "C" {
	int atexit(Base::CrtFunction pf)
	{
		return Base::atexit(pf);
	}

	void __cxa_pure_virtual(void)
	{
		Base::cxa_pure_virtual();
	}

	int mainCRTStartup()
	{
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
