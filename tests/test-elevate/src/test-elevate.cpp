#include <libext/exception.hpp>
#include <libext/priv.hpp>
#include <libbase/std.hpp>
#include <libbase/logger.hpp>
#include <libbase/string.hpp>
#include <libbase/system.hpp>
#include <libbase/filesystem.hpp>
#include <libbase/err.hpp>

#include <tlhelp32.h>

namespace {
	void setup_logger_base()
	{
		using namespace Logger;
		set_default_level(Level::Trace);
		set_default_prefix(Prefix::Medium/* | Prefix::Place | Prefix::Thread | Prefix::Module*/);
		set_default_target(get_TargetToConsole());
//		set_default_target(get_TargetToFile(L".\\log.log"));
	}

	void setup_logger_elevated()
	{
		using namespace Logger;
		set_default_level(Level::Trace);
		set_default_prefix(Prefix::Medium/* | Prefix::Place | Prefix::Thread | Prefix::Module*/);
//		set_default_target(get_TargetToConsole());
		set_default_target(get_TargetToFile(L".\\elevated.log"));
	}

	ustring guid = L"{8D4032BA-28CD-4085-8E7E-592F74EB5D40}";
	ustring pipeName = ustring(L"\\\\.\\pipe\\") + guid;
}

struct Pipe {
	~Pipe()
	{
		::CloseHandle(m_pipe);
	}

	Pipe(PCWSTR name, DWORD openMode, DWORD pipeMode, DWORD maxInstances, LPSECURITY_ATTRIBUTES securityAttributes = nullptr)
	{
		m_pipe = CheckHandle(::CreateNamedPipeW(name, openMode, pipeMode, maxInstances, 0, 0, 0, securityAttributes));
	}

	Pipe(PCWSTR name, size_t /*timeout*/)
	{
		m_pipe = CheckHandle(::CreateFileW(name,
			GENERIC_READ | GENERIC_WRITE,
			0,              // no sharing
			NULL,           // default security attributes
			OPEN_EXISTING,  // opens existing pipe
			SECURITY_SQOS_PRESENT | SECURITY_IMPERSONATION,              // default attributes
			NULL));          // no template file

//		if (m_pipe != INVALID_HANDLE_VALUE)
//			break;
//
//		// Exit if an error other than ERROR_PIPE_BUSY occurs.
//
//		if (GetLastError() != ERROR_PIPE_BUSY)
//		{
//			_tprintf( TEXT("Could not open pipe. GLE=%d\n"), GetLastError() );
//			return -1;
//		}
//
//		// All pipe instances are busy, so wait for 20 seconds.
//
//		if ( ! WaitNamedPipe(lpszPipename, 20000))
//		{
//			printf("Could not open pipe: 20 second wait timed out.");
//			return -1;
//		}
	}

	void connect()
	{
		CheckApi(::ConnectNamedPipe(m_pipe, nullptr));
	}

	void disconnect()
	{
		::DisconnectNamedPipe(m_pipe);
	}

	operator HANDLE() const
	{
		return m_pipe;
	}

private:
	HANDLE m_pipe;
};

int elevated()
{
	setup_logger_elevated();
	LogTrace();

	HANDLE procToken = nullptr;
	CheckApi(::OpenProcessToken(::GetCurrentProcess(), MAXIMUM_ALLOWED, &procToken));
	Ext::WinPriv::modify(procToken, SE_DEBUG_NAME, true);
	Ext::WinPriv::modify(procToken, SE_ASSIGNPRIMARYTOKEN_NAME, true);
	Ext::WinPriv::modify(procToken, SE_TCB_NAME, true);
	Ext::WinPriv::modify(procToken, SE_SECURITY_NAME, true);
	Ext::WinPriv::modify(procToken, SE_CREATE_TOKEN_NAME, true);
	Ext::WinPriv::modify(procToken, SE_IMPERSONATE_NAME, true);

	HANDLE restrictedToken = nullptr;
	CheckApi(::CreateRestrictedToken(procToken, DISABLE_MAX_PRIVILEGE, 0, nullptr, 0, nullptr, 0, nullptr, &restrictedToken));
	LogInfo(L"IsTokenRestricted: %d\n", ::IsTokenRestricted(restrictedToken));

	CheckApi(::ImpersonateLoggedOnUser(restrictedToken));
	CheckApi(Fsys::Directory::create_nt(L"c:\\Program Files\\~elevate1"));
	CheckApi(::RevertToSelf());

//	// Take a snapshot of all running threads
//	HANDLE hThreadSnap = CheckHandle(::CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0));
//
//	THREADENTRY32 te32;
//	te32.dwSize = sizeof(te32);
//
//	// Retrieve information about the first thread,
//	// and exit if unsuccessful
//	if( !Thread32First( hThreadSnap, &te32 ) )
//	{
//		printError( TEXT("Thread32First") );  // Show cause of failure
//		CloseHandle( hThreadSnap );     // Must clean up the snapshot object!
//		return( FALSE );
//	}


	TOKEN_ELEVATION t_TokenElevation;
	DWORD dw_TokenLength = 0;
	CheckApi(::GetTokenInformation(procToken, TokenElevation, &t_TokenElevation, sizeof(t_TokenElevation), &dw_TokenLength));
	LogInfo(L"t_TokenElevation.TokenIsElevated: %u\n", t_TokenElevation.TokenIsElevated);
	TOKEN_ELEVATION_TYPE e_ElevationType;
	if (t_TokenElevation.TokenIsElevated != 0)
	{
		CheckApi(::GetTokenInformation(procToken, TokenElevationType, &e_ElevationType, sizeof(e_ElevationType), &dw_TokenLength));
		LogInfo(L"e_ElevationType: %d\n", (int)e_ElevationType);
//		if (e_ElevationType == TokenElevationTypeFull || e_ElevationType == TokenElevationTypeDefault)
//		{
//		}
	}

	DWORD read = 0;
	DWORD pid = 0;
	DWORD tid = 0;
	HANDLE tokenDup = nullptr;
		Pipe client(pipeName.c_str(), 20000);
		CheckApi(::ReadFile(client, &tid, sizeof(tid), &read, nullptr));

		HANDLE parentThread = CheckHandle(::OpenThread(THREAD_ALL_ACCESS, false, tid));

		::DuplicateTokenEx(procToken, TOKEN_ALL_ACCESS, NULL, SecurityImpersonation, /*TokenPrimary*/ TokenImpersonation, &tokenDup);

		CheckApi(::SetThreadToken(&parentThread, restrictedToken));
		Ext::WinPriv::modify(tokenDup, SE_DEBUG_NAME, true);
		Ext::WinPriv::modify(tokenDup, SE_ASSIGNPRIMARYTOKEN_NAME, true);
		Ext::WinPriv::modify(tokenDup, SE_TCB_NAME, true);
		Ext::WinPriv::modify(tokenDup, SE_SECURITY_NAME, true);
		Ext::WinPriv::modify(tokenDup, SE_CREATE_TOKEN_NAME, true);
		Ext::WinPriv::modify(tokenDup, SE_IMPERSONATE_NAME, true);

//	Pipe client(pipeName.c_str(), 20000);
//	CheckApi(::ReadFile(client, &pid, sizeof(pid), &read, nullptr));
	LogInfo(L"pid: %u\n", pid);
	LogInfo(L"tid: %u\n", tid);


//	HANDLE parent = CheckHandle(::OpenProcess(PROCESS_ALL_ACCESS, false, pid));
	HANDLE target = nullptr;
//	CheckApi(::DuplicateHandle(GetCurrentProcess(), tokenDup, parent, &target, 0, false, DUPLICATE_CLOSE_SOURCE | DUPLICATE_SAME_ACCESS));
//	LogInfo(L"target: %Iu\n", target);

	DWORD written = 0;
	CheckApi(::WriteFile(client, &target, sizeof(target), &written, nullptr));
//	CheckApi(Fsys::Directory::create_nt(L"c:\\Program Files\\~elevate"));
	::Sleep(10000);
	return 0;
}

int base()
{
	setup_logger_base();
	LogTrace();
	ustring modulePath = Base::Process::get_path();

	HANDLE procToken = nullptr;
	CheckApi(::OpenProcessToken(::GetCurrentProcess(), GENERIC_ALL | TOKEN_QUERY | TOKEN_ADJUST_PRIVILEGES, &procToken));
	Ext::WinPriv::modify(procToken, SE_ASSIGNPRIMARYTOKEN_NAME, true);
	Ext::WinPriv::modify(procToken, SE_TCB_NAME, true);
	Ext::WinPriv::modify(procToken, SE_SECURITY_NAME, true);
	Ext::WinPriv::modify(procToken, SE_CREATE_TOKEN_NAME, true);
	Ext::WinPriv::modify(procToken, SE_IMPERSONATE_NAME, true);
//
//	Sleep(20000);
//	Ext::Privilege CreateSymlinkPrivilege(SE_IMPERSONATE_NAME);

	SHELLEXECUTEINFOW info = {
		sizeof(info),
		SEE_MASK_FLAG_NO_UI|SEE_MASK_UNICODE|/*SEE_MASK_NOASYNC|*/SEE_MASK_NOCLOSEPROCESS,
		nullptr,
		L"runas",
		modulePath.c_str(),
		L"param",
		L"path",
	};

	CheckApi(::ShellExecuteExW(&info));

//	WaitForSingleObject(info.hProcess, 3000);
//
//	HANDLE threadHandle = nullptr;
//	CheckApi(::DuplicateHandle(GetCurrentProcess(), GetCurrentThread(), info.hProcess, &threadHandle, GENERIC_ALL, false, 0));
//
	DWORD written = 0;
//	{
//		Pipe pipe(pipeName.c_str(), PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED, PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT, 1);
//		pipe.connect();
//
//		CheckApi(::WriteFile(pipe, &threadHandle, sizeof(threadHandle), &written, nullptr));
//		pipe.disconnect();
//	}
	DWORD pid = ::GetCurrentProcessId();
	DWORD tid = ::GetCurrentThreadId();
	Pipe pipe(pipeName.c_str(), PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED, PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT, 1);
	pipe.connect();

	CheckApi(::ImpersonateNamedPipeClient(pipe));
	{
		HANDLE token = nullptr;
		CheckApi(::OpenThreadToken(::GetCurrentThread(), /*GENERIC_ALL | */TOKEN_QUERY | TOKEN_ADJUST_PRIVILEGES, false, &token));

		SECURITY_IMPERSONATION_LEVEL level = SecurityImpersonation;
		DWORD read = 0;
		CheckApi(::GetTokenInformation(token, TokenImpersonationLevel, &level, sizeof(level), &read));
		LogInfo(L"Security level: %d\n", (int)level);

//		Ext::WinPriv::modify(token, SE_IMPERSONATE_NAME, true);
//		LogInfo(L"is_ena: %d\n", Ext::WinPriv::is_enabled(token, SE_IMPERSONATE_NAME));

	}

	CheckApi(::WriteFile(pipe, &tid, sizeof(tid), &written, nullptr));
//	LogInfo(L"threadHandle: %Iu\n", threadHandle);
	LogInfo(L"pid: %u\n", pid);
	LogInfo(L"tid: %u\n", tid);

//	DWORD read = 0;
//	HANDLE target = nullptr;
//	CheckApi(::ReadFile(pipe, &target, sizeof(target), &read, nullptr));

//	LogInfo(L"target: %Iu\n", target);

//	HANDLE process = CheckHandle(::OpenProcess(GENERIC_READ, false,	data));
//
//	HANDLE token = nullptr;
//	CheckApi(::OpenProcessToken(process, GENERIC_ALL, &token));
//

//	CheckApi(::ImpersonateLoggedOnUser(target));

//	CheckApi(::ImpersonateNamedPipeClient(pipe));

	Sleep(3000);
	{
		HANDLE token = nullptr;
		CheckApi(::OpenThreadToken(::GetCurrentThread(), /*GENERIC_ALL | */TOKEN_QUERY/* | TOKEN_ADJUST_PRIVILEGES*/, false, &token));

		SECURITY_IMPERSONATION_LEVEL level = SecurityImpersonation;
		DWORD read = 0;
		CheckApi(::GetTokenInformation(token, TokenImpersonationLevel, &level, sizeof(level), &read));
		LogInfo(L"level: %d\n", (int)level);
	}

	::Sleep(3000);
	CheckApi(Fsys::Directory::create_nt(L"c:\\Program Files\\~tmp1"));
	Sleep(5000);
	CheckApi(::RevertToSelf());
//	LogErrorIf(!Fsys::Directory::create_nt(L"c:\\Program Files\\tmp2\\"), L"%s\n", Base::ErrAsStr().c_str());

//	DWORD dwError = GetLastError();
//	if (dwError == ERROR_CANCELLED)
//	{
//		// The user refused the elevation.
//		// Do nothing ...
//	}
	pipe.disconnect();

	return 0;
}

int main()
{
	int ret = 0;
	int argc = 0;
	PWSTR * argv = ::CommandLineToArgvW(::GetCommandLineW(), &argc);

	try {
		if (argc > 1)
			ret = elevated();
		else
			ret = base();
	} catch (Ext::AbstractError & e) {
		LogError(L"%s\n", e.what().c_str());
		LogError(L"%s\n", e.where());
	}

	::LocalFree(argv);
	return ret;
}
