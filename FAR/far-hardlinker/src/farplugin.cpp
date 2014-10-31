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

#include <farplugin.hpp>
#include <globalinfo.hpp>
#include <guid.hpp>
#include <lang.hpp>
#include <FileProcessor.hpp>
#include <global.hpp>
#include <fsys.hpp>

#include <far3/plugin.hpp>
#include <far3/message.hpp>
#include <far3/dialog.hpp>

#include <basis/sys/cstr.hpp>
#include <basis/sys/logger.hpp>

struct FarPlugin: public far3::Plugin_i {
	~FarPlugin();

	FarPlugin(const PluginStartupInfo * info);

	void destroy() const override;

	void GetPluginInfo(PluginInfo * info) override;

	far3::PanelController_i* Open(const OpenInfo* info) override;

private:
	mutable WCHAR menu_item[64];
};

FarPlugin::~FarPlugin()
{
}

FarPlugin::FarPlugin(const PluginStartupInfo * info):
	far3::Plugin_i(info)
{
	LogTraceObj();

	global::vars().cryptProvider = crypt::provider();
}

void FarPlugin::destroy() const
{
	delete this;
}

void FarPlugin::GetPluginInfo(PluginInfo * info)
{
	LogTrace();
	info->Flags = PF_EDITOR | PF_VIEWER | PF_DIALOG;

	static GUID PluginMenuGuids[] = {MenuGuid,};
	static PCWSTR PluginMenuStrings[] = {menu_item,};

	cstr::copy(menu_item, far3::message::get(far3::message::MenuTitle), lengthof(menu_item));

	info->PluginMenu.Guids = PluginMenuGuids;
	info->PluginMenu.Strings = PluginMenuStrings;
	info->PluginMenu.Count = lengthof(PluginMenuStrings);

	info->CommandPrefix = get_global_info()->prefix;
}

namespace far3 {
	class Menu {
	public:
		Menu(const GUID& guid):
			m_guid(guid)
		{
			LogTraceObj();
		}

		bool show() {
			LogTraceObj();

			static FarMenuItem items[] = {
				{MIF_NONE, L"qwe1", {0, 0}, 0, {0, 0}},
			};

			auto ptr = far3::psi().Menu(get_plugin_guid(), &m_guid, -1, -1, 0, FMENU_NONE, L"menu", nullptr, nullptr, nullptr, nullptr, items, lengthof(items));
			return ptr;
		}

	private:
		const GUID& m_guid;
	};

}

far3::PanelController_i* FarPlugin::Open(const OpenInfo * info)
{
	UNUSED(info);
	LogTrace();

	using namespace far3;
	auto fgi = get_global_info();
	fgi->load_settings();

	FarListItem cbOperation[] = {
		{0, message::get(lbSearchOnly), {0}},
		{0, message::get(lbHardlinkAuto), {0}},
		{0, message::get(lbHardlinkManual), {0}},
	};

	auto dialog = dialog::create_builder(32, DialogGuid, message::get(message::DlgTitle));
	if (dialog) {
		dialog->add_item(dialog::create_label(-1, txOperation));
		dialog->add_item_after(dialog::create_combobox(fgi->m_cbOperation, cbOperation, lengthof(cbOperation), -1, DIF_DROPDOWNLIST | DIF_LISTNOAMPERSAND));
		dialog->add_item(dialog::create_separator());
	//	dialog->add_item(dialog::create_label(txComparation));
	//	dialog->add_item(dialog::create_combobox(reinterpret_cast<ssize_t*>(&fgi->cbValue_Comparation), cbComparation, Base::lengthof(cbComparation), DIF_DROPDOWNLIST | DIF_LISTNOAMPERSAND));
		dialog->add_item(dialog::create_checkbox(fgi->m_cbMask, -1, cbMask));
		dialog->add_item_after(dialog::create_edit(fgi->m_edMask, 20));
		dialog->add_item(dialog::create_checkbox(fgi->m_cbDoRecursive, -1, cbDoRecursive));
		dialog->add_item(dialog::create_separator(txFileFilters, DIF_CENTERTEXT));
		dialog->add_item(dialog::create_checkbox(fgi->m_cbFilterFileReadOnly, 20, cbFilterFileReadOnly));
		dialog->add_item_after(dialog::create_checkbox(fgi->m_cbFilterDirReadOnly, 20, cbFilterDirReadOnly));
		dialog->add_item(dialog::create_checkbox(fgi->m_cbFilterFileHidden, 20, cbFilterFileHidden));
		dialog->add_item_after(dialog::create_checkbox(fgi->m_cbFilterDirHidden, 20, cbFilterDirHidden));
		dialog->add_item(dialog::create_checkbox(fgi->m_cbFilterFileSystem, 20, cbFilterFileSystem));
		dialog->add_item_after(dialog::create_checkbox(fgi->m_cbFilterDirSystem, 20, cbFilterDirSystem));
		dialog->add_item(dialog::create_checkbox(fgi->m_cbFilterFileLink, 20, cbFilterFileLink));
		dialog->add_item_after(dialog::create_checkbox(fgi->m_cbFilterDirLink, 20, cbFilterDirLink));
		dialog->add_item(dialog::create_separator());
		dialog->add_item(dialog::create_checkbox(fgi->m_cbFilterFileSize, -1, cbFilterFileSize));
	//	dialog->add_item(dialog::create_separator(txFileRestrictions, DIF_CENTERTEXT));
	//	dialog->add_item(dialog::create_checkbox(fgi->m_cbRestrictionFileTime, cbRestrictionFileTime));
	//	dialog->add_item(dialog::create_checkbox(fgi->m_cbRestrictionFileAttributes, cbRestrictionFileAttributes));
	//	dialog->add_item(dialog::create_separator());
	//	dialog->add_item(dialog::create_label(txWhitespace));
	//	dialog->add_item_after(dialog::create_edit(fgi->edValue_Whitespaces, 10));
	//	dialog->add_item(dialog::create_separator());
	//	dialog->add_item(dialog::create_checkbox(fgi->cbValue_Selected, cbSelected, (fgi->get_block_type() != BTYPE_COLUMN) ? DIF_DISABLE : 0));
	//	dialog->add_item(dialog::create_checkbox(fgi->cbValue_AsEmpty, cbAsEmpty, (fgi->get_block_type() != BTYPE_COLUMN) ? DIF_DISABLE : 0));
		dialog->add_item(dialog::create_separator());
		dialog->add_OKCancel(message::get(message::txtBtnOk), message::get(message::txtBtnCancel));
		LogTrace();
		if (dialog->show() == 0) {
			fgi->save_settings();

			global::vars().folders.emplace_back(fsys::Node_t(new fsys::Folder(L"c:\\sysint")));
			global::vars().folders.emplace_back(fsys::Node_t(new fsys::Folder(L"d:\\sysint")));

			FileProcessor().execute();
		}
	}
	LogTrace();

	return nullptr;
}

///=================================================================================================
far3::Plugin_i* create_FarPlugin(const PluginStartupInfo* psi)
{
	return new FarPlugin(psi);
}
