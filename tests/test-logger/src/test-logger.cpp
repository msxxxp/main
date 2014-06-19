#include <basis/sys/console.hpp>
#include <basis/sys/crt.hpp>
#include <basis/sys/logger.hpp>
#include <basis/sys/sstr.hpp>
#include <basis/sys/totext.hpp>

#include <basis/std/algorithm>
#include <basis/std/vector>
#include <basis/std/string>

namespace {
	void setup_logger()
	{
		LogSetOptions(L"logger:///default?level=tr;prefix=fu;target=co");
//		LogSetOptions(L"logger:///default?level=tr;prefix=fu;target=fo(test-threads.log)");

		LogTrace();
		LogDebug(L"\n");
		LogInfo(L"\n");
		LogReport(L"\n");
		LogAtten(L"\n");
		LogWarn(L"\n");
		LogError(L"\n");
		LogFatal(L"\n");
		LogAlert(L"\n");
		LogEmerg(L"\n");
		LogForce(L"\n");
	}

	LONG WINAPI unhandledExecptionFilter(PEXCEPTION_POINTERS ep)
	{
		LogFatal(L"terminating process %s\n", totext::nt_status(ep->ExceptionRecord->ExceptionCode).c_str());

		return EXCEPTION_EXECUTE_HANDLER; // should terminate process.
	}

	void do_acces_violation()
	{
		LogWarn(L"accessing null pointer\n");
		volatile int* p = nullptr;
		*p = 0;
	}
}

int WINAPI wWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, LPWSTR /*lpCmdLine*/, int /*nShowCmd*/)
{
	console::printf(L"%S:%d\n", __PRETTY_FUNCTION__, __LINE__);

	::SetUnhandledExceptionFilter(unhandledExecptionFilter);

	setup_logger();

	LogTrace();

	do_acces_violation();

	LogTrace();
	return 0;
}

extern "C" int wmain(int argc, wchar_t * argv[])
{
	console::printf(L"%S:%d\n", __PRETTY_FUNCTION__, __LINE__);

	::SetUnhandledExceptionFilter(unhandledExecptionFilter);

	setup_logger();

	LogTrace();
	LogDebug(L"argc: %d\n", argc);
	for (int i = 0; i < argc; ++i)
		LogDebug(L"%s\n", argv[i]);

	do_acces_violation();

	LogTrace();
	return 0;
}


/// ========================================================================== Startup (entry point)
#ifdef NDEBUG
extern "C" {
	int mainCRTStartup()
	{
		console::printf(L"%S:%d\n", __PRETTY_FUNCTION__, __LINE__);
		crt::init_atexit();

		int argc = 0;
		wchar_t ** argv = ::CommandLineToArgvW(::GetCommandLineW(), &argc);

		int Result = wmain(argc, argv);

		::LocalFree(argv);
		crt::invoke_atexit();
		::ExitProcess(Result);
		return Result;
	}

	int	WinMainCRTStartup() // -mwindows
	{
		console::printf(L"%S:%d\n", __PRETTY_FUNCTION__, __LINE__);
		crt::init_atexit();

		int ret = 0;

		STARTUPINFOW startupInfo;
		::RtlSecureZeroMemory(&startupInfo, sizeof(startupInfo));
		startupInfo.cb = sizeof(startupInfo);
		::GetStartupInfoW(&startupInfo);

		ret = wWinMain(::GetModuleHandleW(nullptr), nullptr, ::GetCommandLineW(), startupInfo.dwFlags & STARTF_USESHOWWINDOW ? startupInfo.wShowWindow : SW_SHOWDEFAULT);

		crt::invoke_atexit();
		::ExitProcess(ret);
		return ret;
	}

	BOOL WINAPI	DllMainCRTStartup(HANDLE, DWORD dwReason, PVOID)
	{
		console::printf(L"%S:%d\n", __PRETTY_FUNCTION__, __LINE__);
		switch (dwReason) {
			case DLL_PROCESS_ATTACH:
				crt::init_atexit();
				break;

			case DLL_PROCESS_DETACH:
				crt::invoke_atexit();
				break;
		}
		return true;
	}

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
