
/**
 © 2014 Andrew Grechkin
 Source code: <http://code.google.com/p/andrew-grechkin>

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program. If not, see <http://www.gnu.org/licenses/>.
 **/

#ifndef _FAR3_HELPER_HPP_
#define _FAR3_HELPER_HPP_

#include <basis/configure.hpp>
#include <far3/~3rdparty/plugin.hpp>
#include <far3/~plugin/fwd.hpp>

//#include <basis/sys/cstr.hpp>
//#include <basis/sys/memory.hpp>

namespace far3 {

	///==================================================================================== helper_t
	struct helper_t {
		static helper_t & inst();

		~helper_t();

		helper_t & init(GlobalInfo_i * gi);

		const GUID * get_guid() const;

		const PluginStartupInfo & psi() const;

		const FarStandardFunctions & fsf() const;

		GlobalInfo_i * get_global_info() const;

		Plugin_i * get_plugin() const;

	private:
		helper_t();

		GlobalInfo_i* m_gi;
	};

	inline const GUID* get_plugin_guid()
	{
		return helper_t::inst().get_guid();
	}

	inline const PluginStartupInfo & psi()
	{
		return helper_t::inst().psi();
	}

	inline const FarStandardFunctions & fsf()
	{
		return helper_t::inst().fsf();
	}

	///=============================================================================================
	void ibox(const wchar_t* text, const wchar_t* tit = L"Info");

	void mbox(const wchar_t* text, const wchar_t* tit = L"Message");

	void ebox(const wchar_t* text, const wchar_t* tit = L"Error");

	void ebox(const wchar_t* msgs[], size_t size, const wchar_t* help = nullptr);

//	void ebox(const cstr::mstring & msg, const wchar_t* title = EMPTY_STR);

	void ebox(DWORD err);

	bool question(const wchar_t* text, const wchar_t* tit);

//	///================================================================================= KeyAction_t
//	typedef bool (PanelController_i::*PanelMemFun)();
//
//	struct KeyAction_t {
//		FarKey Key;
//		const wchar_t* Text;
//		const wchar_t* LongText;
//		PanelMemFun Handler;
//	};

///============================================================================== ProgressWindow
	struct ProgressWindow {
		ProgressWindow(size_t /*size*/, const wchar_t* /*title*/)
		{
		}

		void set_name(size_t /*num*/, const wchar_t* /*name*/)
		{
		}
	private:
	};

//	inline uint64_t get_panel_settings() {
//		return psi().AdvControl(get_plugin_guid(), ACTL_GETPANELSETTINGS, 0, nullptr);
//	} GetSetting(FSSF_PANEL,L"ShowHidden")?true:false;

//	inline uint64_t get_interface_settings() {
//		return psi().AdvControl(get_plugin_guid(), ACTL_GETINTERFACESETTINGS, 0, nullptr);
//	}

}

#endif
