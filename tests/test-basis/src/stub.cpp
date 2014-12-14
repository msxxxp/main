/// ========================================================================== Startup (entry point)
#ifdef NDEBUG
#include <basis/sys/console.hpp>
#include <basis/sys/crt.hpp>
#include <basis/sys/memory.hpp>

extern "C" int wmain(int argc, wchar_t* argv[]);

extern int WINAPI wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int);

namespace {

	void prolog()
	{
		console::printf("%s:%d\n", __PRETTY_FUNCTION__, __LINE__);

		crt::init_atexit();
	}

	int epilog(int errcode)
	{
		console::printf("%s:%d\n", __PRETTY_FUNCTION__, __LINE__);

		crt::invoke_atexit();

		{
			const memory::heap::Stat& stat = memory::heap::DefaultStat::get_stat();
			console::printf("stat alloc: %I64u, %I64u \n", stat.allocations, stat.allocSize);
			console::printf("stat free : %I64u, %I64u \n", stat.frees, stat.freeSize);
			console::printf("stat diff : %I64d \n", stat.allocSize - stat.freeSize);
		}

		::ExitProcess(errcode);
		return errcode;
	}

}

extern "C" {

	int mainCRTStartup()
	{
		console::printf("%s:%d\n", __PRETTY_FUNCTION__, __LINE__);

		prolog();

		int argc = 0;
		wchar_t ** argv = ::CommandLineToArgvW(::GetCommandLineW(), &argc);

		int ret = wmain(argc, argv);

		::LocalFree(argv);

		return epilog(ret);
	}

	int	WinMainCRTStartup() // -mwindows
	{
		prolog();

		STARTUPINFOW startupInfo;
		::RtlSecureZeroMemory(&startupInfo, sizeof(startupInfo));
		startupInfo.cb = sizeof(startupInfo);
		::GetStartupInfoW(&startupInfo);

		int ret = wWinMain(::GetModuleHandleW(nullptr), nullptr, ::GetCommandLineW(), startupInfo.dwFlags & STARTF_USESHOWWINDOW ? startupInfo.wShowWindow : SW_SHOWDEFAULT);

		return epilog(ret);
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
