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

#include <basis/sys/logger.hpp>

#include <libfar3/helper.hpp>
#include <libfar3/dialog_builder.hpp>
#include <libfar3/dialog.hpp>

#include <farplugin.hpp>
#include <globalinfo.hpp>
#include <guid.hpp>
#include <lang.hpp>
#include <FileProcessor.hpp>
#include <global.hpp>
#include <fsys.hpp>

///======================================================================================= FarPlugin
struct FarPlugin: public Far::Plugin_i {
	~FarPlugin();

	FarPlugin(const PluginStartupInfo * info);

	void destroy() const override;

	void GetPluginInfo(PluginInfo * info) override;

	Far::PanelController_i * Open(const OpenInfo * info) override;

private:
	mutable WCHAR menu_item[64];
};

FarPlugin::~FarPlugin()
{
	LogTraceObj();
}

FarPlugin::FarPlugin(const PluginStartupInfo * info):
	Far::Plugin_i(info)
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

	cstr::copy(menu_item, Far::get_msg(Far::MenuTitle), lengthof(menu_item));

	info->PluginMenu.Guids = PluginMenuGuids;
	info->PluginMenu.Strings = PluginMenuStrings;
	info->PluginMenu.Count = lengthof(PluginMenuStrings);

	info->CommandPrefix = get_global_info()->prefix;
}

namespace Far {
	class FoundDialog: Dialog {
	public:
		~FoundDialog();
		FoundDialog();

		void show();

	private:
		static intptr_t handler(HANDLE dlg, intptr_t msg, intptr_t param1, void* param2);

		HANDLE m_hndl;
		int m_width;
		int m_height;
	};

	FoundDialog::FoundDialog():
		m_hndl(nullptr),
		m_width(),
		m_height()
	{
	}

	void FoundDialog::show()
	{
		//		int width = ScrX + 1 - 2;
		//		int height = ScrY + 1 - 2;
		m_width = 50;
		m_height = 15;
		FarDialogItem FindDlgData[]=
		{
			{DI_DOUBLEBOX, 3, 1,  m_width-4, m_height-2, 0, nullptr, nullptr, DIF_SHOWAMPERSAND, title.data()},
			{DI_LISTBOX,   4, 2,  m_width-5, m_height-7, 0, nullptr, nullptr, DIF_LISTNOBOX | DIF_DISABLE, 0},
			{DI_TEXT,     -1, m_height-6, 0, m_height-6, 0, nullptr, nullptr, DIF_SEPARATOR2, L""},
			{DI_BUTTON,    0, m_height-3, 0, m_height-3, 0, nullptr, nullptr, DIF_FOCUS | DIF_DEFAULTBUTTON | DIF_CENTERGROUP, MSG(MFindNewSearch)},
			{DI_BUTTON,    0, m_height-3, 0, m_height-3, 0, nullptr, nullptr, DIF_CENTERGROUP | DIF_DISABLE, MSG(MFindGoTo)},
			{DI_BUTTON,    0, m_height-3, 0, m_height-3, 0, nullptr, nullptr, DIF_CENTERGROUP | DIF_DISABLE, MSG(MFindView)},
			{DI_BUTTON,    0, m_height-3, 0, m_height-3, 0, nullptr, nullptr, DIF_CENTERGROUP | DIF_DISABLE, MSG(MFindPanel)},
			{DI_BUTTON,    0, m_height-3, 0, m_height-3, 0, nullptr, nullptr, DIF_CENTERGROUP, MSG(MFindStop)},
		};

		psi().DialogInit(get_plugin_guid(), ListMenuGuid, -1, -1, m_width, m_height, nullptr, FindDlgData, lengthof(FindDlgData), nullptr, FDLG_NONE, handler, nullptr);
	}

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

			auto ptr = Far::psi().Menu(get_plugin_guid(), &m_guid, -1, -1, 0, FMENU_NONE, L"menu", nullptr, nullptr, nullptr, nullptr, items, lengthof(items));
			return ptr;
		}

	private:
		const GUID& m_guid;
	};

}


Far::PanelController_i * FarPlugin::Open(const OpenInfo * info)
{
	UNUSED(info);
	LogTrace();

	using namespace Far;
	auto fgi = get_global_info();
	fgi->load_settings();

	FarListItem cbOperation[] = {
		{0, get_msg(lbSearchOnly), {0}},
		{0, get_msg(lbHardlinkAuto), {0}},
		{0, get_msg(lbHardlinkManual), {0}},
	};

	Far::Menu menu(ListMenuGuid);
	menu.show();

	return nullptr;

	auto dialog = create_dialog_builder(DialogGuid, get_msg(DlgTitle));
	LogTrace();
	dialog->add_item(create_label(txOperation));
	dialog->add_item_after(create_combobox(reinterpret_cast<ssize_t*>(&fgi->m_cbOperation), cbOperation, lengthof(cbOperation), DIF_DROPDOWNLIST | DIF_LISTNOAMPERSAND));
	dialog->add_item(create_separator());
//	dialog->add_item(create_label(txComparation));
//	dialog->add_item(create_combobox(reinterpret_cast<ssize_t*>(&fgi->cbValue_Comparation), cbComparation, Base::lengthof(cbComparation), DIF_DROPDOWNLIST | DIF_LISTNOAMPERSAND));
	dialog->add_item(create_checkbox(&fgi->m_cbMask, cbMask));
	dialog->add_item_after(create_edit(&fgi->m_edMask, 20));
	dialog->add_item(create_checkbox(&fgi->m_cbDoRecursive, cbDoRecursive));
	dialog->add_item(create_separator(txFileFilters, DIF_CENTERTEXT));
	dialog->add_item(create_checkbox(&fgi->m_cbFilterFileReadOnly, cbFilterFileReadOnly));
	dialog->add_item_after(create_checkbox(&fgi->m_cbFilterDirReadOnly, cbFilterDirReadOnly));
	dialog->add_item(create_checkbox(&fgi->m_cbFilterFileHidden, cbFilterFileHidden));
	dialog->add_item_after(create_checkbox(&fgi->m_cbFilterDirHidden, cbFilterDirHidden));
	dialog->add_item(create_checkbox(&fgi->m_cbFilterFileSystem, cbFilterFileSystem));
	dialog->add_item_after(create_checkbox(&fgi->m_cbFilterDirSystem, cbFilterDirSystem));
	dialog->add_item(create_checkbox(&fgi->m_cbFilterFileLink, cbFilterFileLink));
	dialog->add_item_after(create_checkbox(&fgi->m_cbFilterDirLink, cbFilterDirLink));
	dialog->add_item(create_separator());
	dialog->add_item(create_checkbox(&fgi->m_cbFilterFileSize, cbFilterFileSize));
//	dialog->add_item(create_separator(txFileRestrictions, DIF_CENTERTEXT));
//	dialog->add_item(create_checkbox(&fgi->m_cbRestrictionFileTime, cbRestrictionFileTime));
//	dialog->add_item(create_checkbox(&fgi->m_cbRestrictionFileAttributes, cbRestrictionFileAttributes));
//	dialog->add_item(create_separator());
//	dialog->add_item(create_label(txWhitespace));
//	dialog->add_item_after(create_edit(fgi->edValue_Whitespaces, 10));
//	dialog->add_item(create_separator());
//	dialog->add_item(create_checkbox(&fgi->cbValue_Selected, cbSelected, (fgi->get_block_type() != BTYPE_COLUMN) ? DIF_DISABLE : 0));
//	dialog->add_item(create_checkbox(&fgi->cbValue_AsEmpty, cbAsEmpty, (fgi->get_block_type() != BTYPE_COLUMN) ? DIF_DISABLE : 0));
	dialog->add_item(create_separator());
	dialog->add_OKCancel(get_msg(txtBtnOk), get_msg(txtBtnCancel));
	LogTrace();
	if (dialog->show()) {
		fgi->save_settings();

		global::vars().folders.emplace_back(fsys::Node_t(new fsys::Folder(L"c:\\sysint")));
		global::vars().folders.emplace_back(fsys::Node_t(new fsys::Folder(L"d:\\sysint")));

		FileProcessor().execute();
	}
	LogTrace();

	return nullptr;
}

///=================================================================================================
Far::Plugin_i * create_FarPlugin(const PluginStartupInfo * psi)
{
	return new FarPlugin(psi);
}
