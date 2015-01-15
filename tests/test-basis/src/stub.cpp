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
		TraceFunc();
		crt::init_atexit();
		TraceFunc();
	}

	int epilog(int errcode)
	{
		TraceFunc();
		crt::invoke_atexit();

		{
			const auto stat = memory::heap::DefaultStat::get_stat();
			console::printf("Heap '%s' statistics:\n", memory::heap::DefaultStat::get_name());
			console::printf("  alloc: %I64u, %I64u\n", stat.get_allocations(), stat.get_allocations_size());
			console::printf("  free : %I64u, %I64u\n", stat.get_frees(), stat.get_frees_size());
			console::printf("  diff : %I64d\n", stat.get_allocations_size() - stat.get_frees_size());
		}

		TraceFunc();
		::ExitProcess(errcode);
		return errcode;
	}

}

extern "C" {

	int mainCRTStartup()
	{
		TraceFunc();

		prolog();

		int argc = 0;
		wchar_t ** argv = ::CommandLineToArgvW(::GetCommandLineW(), &argc);

		int ret = wmain(argc, argv);

		::LocalFree(argv);

		TraceFunc();
		return epilog(ret);
	}

	int	WinMainCRTStartup() // -mwindows
	{
		TraceFunc();

		prolog();

		STARTUPINFOW startupInfo;
		::RtlSecureZeroMemory(&startupInfo, sizeof(startupInfo));
		startupInfo.cb = sizeof(startupInfo);
		::GetStartupInfoW(&startupInfo);

		int ret = wWinMain(::GetModuleHandleW(nullptr), nullptr, ::GetCommandLineW(), startupInfo.dwFlags & STARTF_USESHOWWINDOW ? startupInfo.wShowWindow : SW_SHOWDEFAULT);

		TraceFunc();
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
		TraceFunc();
		return crt::atexit(pf);
	}

	void __cxa_pure_virtual(void)
	{
		TraceFunc();
		crt::cxa_pure_virtual();
	}

}
#endif
