/// ========================================================================== Startup (entry point)
#ifdef NDEBUG
#include <basis/sys/console.hpp>
#include <basis/sys/crt.hpp>
#include <basis/sys/linkage.hpp>
#include <basis/sys/memory.hpp>

extern "C" int wmain(int argc, wchar_t* argv[]);

extern int WINAPI wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int);

extern "C" {

	LONG WINAPI RedirectedSetUnhandledExceptionFilter(EXCEPTION_POINTERS * /*ExceptionInfo*/)
	{
		// When the CRT calls SetUnhandledExceptionFilter with NULL parameter
		// our handler will not get removed.

		console::printf("%s:%d\n", __PRETTY_FUNCTION__, __LINE__);

		return 0;
	}

	int mainCRTStartup()
	{
		int ret = 0;

		console::printf(L"%S:%d\n", __PRETTY_FUNCTION__, __LINE__);

//		linkage::CAPIHook apiHook1("kernel32.dll", "SetUnhandledExceptionFilter", (PROC)RedirectedSetUnhandledExceptionFilter);

		crt::init_atexit();

		int argc = 0;
		wchar_t ** argv = ::CommandLineToArgvW(::GetCommandLineW(), &argc);

		ret = wmain(argc, argv);

		::LocalFree(argv);
		crt::invoke_atexit();

		{
			const memory::heap::Stat& stat = memory::heap::DefaultStat::get_stat();
			console::printf(L"stat alloc: %I64u, %I64u \n", stat.allocations, stat.allocSize);
			console::printf(L"stat free : %I64u, %I64u \n", stat.frees, stat.freeSize);
			console::printf(L"stat diff : %I64d \n", stat.allocSize - stat.freeSize);
		}
		::ExitProcess(ret);
		return ret;
	}

	int	WinMainCRTStartup() // -mwindows
	{
		int ret = 0;

		console::printf(L"%S:%d\n", __PRETTY_FUNCTION__, __LINE__);
		crt::init_atexit();

		STARTUPINFOW startupInfo;
		::RtlSecureZeroMemory(&startupInfo, sizeof(startupInfo));
		startupInfo.cb = sizeof(startupInfo);
		::GetStartupInfoW(&startupInfo);

		ret = wWinMain(::GetModuleHandleW(nullptr), nullptr, ::GetCommandLineW(), startupInfo.dwFlags & STARTF_USESHOWWINDOW ? startupInfo.wShowWindow : SW_SHOWDEFAULT);

		crt::invoke_atexit();

		{
			const memory::heap::Stat& stat = memory::heap::DefaultStat::get_stat();
			console::printf(L"stat alloc: %I64u, %I64u \n", stat.allocations, stat.allocSize);
			console::printf(L"stat free : %I64u, %I64u \n", stat.frees, stat.freeSize);
			console::printf(L"stat diff : %I64d \n", stat.allocSize - stat.freeSize);
		}
		::ExitProcess(ret);
		return ret;
	}

//	BOOL WINAPI	DllMainCRTStartup(HANDLE, DWORD dwReason, PVOID)
//	{
//		console::printf(L"%S:%d\n", __PRETTY_FUNCTION__, __LINE__);
//		switch (dwReason) {
//			case DLL_PROCESS_ATTACH:
//				crt::init_atexit();
//				break;
//
//			case DLL_PROCESS_DETACH:
//				crt::invoke_atexit();
//				break;
//		}
//		return true;
//	}

	int atexit(crt::Function pf)
	{
		return crt::atexit(pf);
	}

	void __cxa_pure_virtual(void)
	{
		crt::cxa_pure_virtual();
	}

}
#endif
