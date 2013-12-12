/**
 hidcon: Hidden console
 Command line tool to run hidden console applications

 © 2013 Andrew Grechkin

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/

#include <libbase/std.hpp>
#include <libbase/cstr.hpp>
#include <shlwapi.h>

const wchar_t * CMD_DETACH = L"/detach";

int APIENTRY wWinMain(HINSTANCE, HINSTANCE, PWSTR pCmdLine, int)
{
	DWORD ret = 0;
	int argc = 0;
	PWSTR * argv = ::CommandLineToArgvW(pCmdLine, &argc);

	if (argc > 1) {
		bool detach = false;
		for (int i = 1; i < argc; ++i) {
			wchar_t buf[MAX_PATH];
			Cstr::copy(buf, argv[i], Base::lengthof(buf));
			Cstr::Inplace::to_lower(buf);
			if (Cstr::compare(CMD_DETACH, buf) == 0)
				detach = true;
		}

		PROCESS_INFORMATION pi;
		STARTUPINFOW si;

		Memory::zero(pi);
		Memory::zero(si);

		si.cb = sizeof(si);
		si.wShowWindow = SW_HIDE;
		si.dwFlags = STARTF_USESHOWWINDOW;

		wchar_t cmdl[Base::MAX_PATH_LEN];
		wchar_t path[Base::MAX_PATH_LEN];

		::ExpandEnvironmentStringsW(argv[argc - 1], cmdl, Base::lengthof(cmdl));
		::PathCanonicalizeW(path, cmdl);

		if (::CreateProcessW(nullptr, path, nullptr, nullptr, false, CREATE_NO_WINDOW, nullptr, nullptr, &si, &pi)) {
			::CloseHandle(pi.hThread);
			::WaitForSingleObject(pi.hProcess, detach ? 100 : INFINITE);
			::GetExitCodeProcess(pi.hProcess, &ret);
			::CloseHandle(pi.hProcess);
		} else {
			ret = ::GetLastError();
		}
	} else {
		ret = 1;
		Base::mbox(L"hidcon: Execute hidden console window\n"
		           L"Use: hidcon [/detach] \"<command_line>\"\n"
		           L"\t/detach - hidcon won't wait for application\n"
		           L"\tand you won't be able to get the exitcode\n"
		           L"Example: hidcon \"cmd /c ping localhost > c:\\ping.log\"",
		           L"hidcon");
	}

	::LocalFree(argv); // do not replace or move
	return ret;
}

///=========================================================================== Startup (entry point)
extern "C" int WinMainCRTStartup()
{
	int ret;
//	::ShowWindow(GetConsoleWindow(), SW_MINIMIZE);
	STARTUPINFOW StartupInfo = {sizeof(STARTUPINFO), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	::GetStartupInfoW(&StartupInfo);

	ret = wWinMain(::GetModuleHandleW(nullptr), nullptr, ::GetCommandLineW(), StartupInfo.dwFlags & STARTF_USESHOWWINDOW ? StartupInfo.wShowWindow : SW_SHOWDEFAULT);

	::ExitProcess(ret);
	return ret;
}
