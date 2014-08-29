/**
	hardlinker: Search duplicates and make hardlinks
	FAR3-lua plugin

	© 2014 Andrew Grechkin

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

#include <globalinfo.hpp>
#include <farplugin.hpp>

#include <libfar3/helper.hpp>
#include <libfar3/plugin_i.hpp>

#include <basis/sys/logger.hpp>

namespace {
	void setup_logger()
	{
		LogSetOptions(L"logger:///default?level=tr;prefix=fu;target=fo(hardlinker.log)");
//		LogSetOptions(L"logger:///module?name=user_console;prefix=0;level=fa;target=n");
	}
}


///========================================================================================== Export
/// GlobalInfo
void WINAPI GetGlobalInfoW(GlobalInfo * info)
{
	setup_logger();

	LogTrace();
	Far::helper_t::inst().init(new FarGlobalInfo);
	get_global_info()->GetGlobalInfoW(info);
}

void WINAPI SetStartupInfoW(const PluginStartupInfo * info)
{
	LogTrace();
	get_global_info()->SetStartupInfoW(info);
	get_global_info()->load_settings();
}

intptr_t WINAPI ConfigureW(const ConfigureInfo * info)
{
	LogTrace();
	return get_global_info()->ConfigureW(info);
}

/// Plugin
void WINAPI GetPluginInfoW(PluginInfo * info)
{
	LogTrace();
	Far::helper_t::inst().get_plugin()->GetPluginInfoW(info);
}

HANDLE WINAPI OpenW(const OpenInfo * info)
{
	LogTrace();
	return Far::helper_t::inst().get_plugin()->OpenW(info);
}

void WINAPI ExitFARW(const ExitInfo *info)
{
	LogTrace();
	Far::helper_t::inst().get_plugin()->ExitFARW(info);
}

/// Panel

///=========================================================================== Startup (entry point)
#ifdef NDEBUG
extern "C" {
	BOOL WINAPI	DllMainCRTStartup(HANDLE, DWORD dwReason, PVOID) {
		switch (dwReason) {
			case DLL_PROCESS_ATTACH:
				crt::init_atexit();;
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
