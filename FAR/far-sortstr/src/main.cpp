/**
	sortstr: Sort strings in editor
	FAR3 plugin

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

#include <globalinfo.hpp>
#include <farplugin.hpp>

#include <libfar3/helper.hpp>
#include <libfar3/plugin_i.hpp>
#include <liblog/logger.hpp>
#include <libbase/atexit.hpp>

namespace {
	void setup_logger()
	{
		using namespace Logger;
		set_default_level(Level::Trace);
		set_default_prefix(Prefix::Medium | Prefix::Place);
		set_default_target(get_TargetToFile(L"D:/projects/~test/sortstr.log", true));
	}
}


///========================================================================================== Export
/// GlobalInfo
void WINAPI GetGlobalInfoW(GlobalInfo * Info)
{
	setup_logger();

	LogTrace();
	Far::helper_t::inst().init(new FarGlobalInfo);
	get_global_info()->GetGlobalInfoW(Info);
}

void WINAPI SetStartupInfoW(const PluginStartupInfo * Info)
{
	LogTrace();
	get_global_info()->SetStartupInfoW(Info);
	get_global_info()->load_settings();
}

intptr_t WINAPI ConfigureW(const ConfigureInfo * Info)
{
	LogTrace();
	return get_global_info()->ConfigureW(Info);
}

/// Plugin
void WINAPI GetPluginInfoW(PluginInfo * Info)
{
	LogTrace();
	Far::helper_t::inst().get_plugin()->GetPluginInfoW(Info);
}

HANDLE WINAPI OpenW(const OpenInfo * Info)
{
	LogTrace();
	return Far::helper_t::inst().get_plugin()->OpenW(Info);
}

void WINAPI ExitFARW(const ExitInfo * Info)
{
	LogTrace();
	Far::helper_t::inst().get_plugin()->ExitFARW(Info);
}

/// Panel

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

	void _pei386_runtime_relocator()
	{
	}

	BOOL WINAPI	DllMainCRTStartup(HANDLE, DWORD dwReason, PVOID) {
		switch (dwReason) {
			case DLL_PROCESS_ATTACH:
				Base::init_atexit();
				break;

			case DLL_PROCESS_DETACH:
				Base::invoke_atexit();
				break;
		}
		return true;
	}

}
#endif
