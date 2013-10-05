#include <windows.h>
#include <stdio.h>

int wWinMain(const wchar_t * pCmdLine);

#ifdef DEBUG

int main()
{
	printf("%s\n", __PRETTY_FUNCTION__);
	return wWinMain(::GetCommandLineW());
}
#else
extern "C" int WinMainCRTStartup()
{
//	printf("%s\n", __PRETTY_FUNCTION__);

	int Result = 0;
	STARTUPINFO StartupInfo = {sizeof(STARTUPINFO), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	::GetStartupInfo(&StartupInfo);

	Result = wWinMain(::GetCommandLineW());
	::ExitProcess(Result);
	return	Result;
}

extern "C" void __cxa_pure_virtual(void)
{
//	printf("%s\n", __PRETTY_FUNCTION__);
	//		::abort_message("pure virtual method called");
}

#endif
