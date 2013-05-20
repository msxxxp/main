/**
	filever: File Version FAR plugin
	Displays version information from file resource in dialog
	FAR3lua plugin

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

#include <farplugin.hpp>

#include <libfar3/helper.hpp>
#include <libfar3/panel.hpp>
#include <libfar3/viewer.hpp>
#include <libfar3/obsolete.hpp>
#include <libbase/logger.hpp>
#include <libbase/memory.hpp>
#include <libbase/pcstr.hpp>
#include <libbase/path.hpp>

#include <globalinfo.hpp>
#include <guid.hpp>
#include <lang.hpp>
#include <fileversion.hpp>

#include <time.h>


Base::NamedValues<WORD> Machines[] = {
	{ L"UNKNOWN", IMAGE_FILE_MACHINE_UNKNOWN },
	{ L"I386", IMAGE_FILE_MACHINE_I386 },
	{ L"R4000", IMAGE_FILE_MACHINE_R4000 },
	{ L"WCEMIPSV2", IMAGE_FILE_MACHINE_WCEMIPSV2 },
	{ L"SH3", IMAGE_FILE_MACHINE_SH3 },
	{ L"SH3DSP", IMAGE_FILE_MACHINE_SH3DSP },
	{ L"SH4", IMAGE_FILE_MACHINE_SH4 },
	{ L"SH5", IMAGE_FILE_MACHINE_SH5 },
	{ L"ARM", IMAGE_FILE_MACHINE_ARM },
	{ L"THUMB", IMAGE_FILE_MACHINE_THUMB },
	{ L"AM33", IMAGE_FILE_MACHINE_AM33 },
	{ L"POWERPC", IMAGE_FILE_MACHINE_POWERPC },
	{ L"POWERPCFP", IMAGE_FILE_MACHINE_POWERPCFP },
	{ L"IA64", IMAGE_FILE_MACHINE_IA64 },
	{ L"MIPS16", IMAGE_FILE_MACHINE_MIPS16 },
	{ L"MIPSFPU", IMAGE_FILE_MACHINE_MIPSFPU },
	{ L"MIPSFPU16", IMAGE_FILE_MACHINE_MIPSFPU16 },
	{ L"EBC", IMAGE_FILE_MACHINE_EBC },
	{ L"AMD64", IMAGE_FILE_MACHINE_AMD64 },
	{ L"M32R", IMAGE_FILE_MACHINE_M32R },
};

///======================================================================================= FarPlugin
struct FarPlugin: public Far::Plugin_i {
	FarPlugin(const PluginStartupInfo * Info);

	~FarPlugin() override;

	void GetPluginInfo(PluginInfo * Info) override;

	Far::PanelController_i * Open(const OpenInfo * Info) override;
};

FarPlugin::FarPlugin(const PluginStartupInfo * Info):
	Far::Plugin_i(Info)
{
	LogTrace();
}

FarPlugin::~FarPlugin()
{
	LogTrace();
}

void FarPlugin::GetPluginInfo(PluginInfo * Info)
{
	LogTrace();
	Info->Flags = PF_VIEWER;

	static GUID PluginMenuGuids[] = {MenuGuid,};
	static PCWSTR PluginMenuStrings[] = {Far::get_msg(Far::MenuTitle),};
	PluginMenuStrings[0] = Far::get_msg(Far::MenuTitle);

	Info->PluginMenu.Guids = PluginMenuGuids;
	Info->PluginMenu.Strings = PluginMenuStrings;
	Info->PluginMenu.Count = Base::lengthof(PluginMenuStrings);

	Info->CommandPrefix = get_global_info()->prefix;
}

Far::PanelController_i * FarPlugin::Open(const OpenInfo * Info)
{
	LogNoise(L"OpenFrom: %d\n", (int)Info->OpenFrom);
	if (!version_dll::inst().is_valid()) {
		Far::ebox(L"Can't load version.dll");
		return (Far::PanelController_i * )INVALID_HANDLE_VALUE;
	}

	WCHAR buf1[Base::MAX_PATH_LEN] = {0};
	WCHAR buf2[Base::MAX_PATH_LEN] = {0};
	if (Info->OpenFrom == OPEN_PLUGINSMENU || Info->OpenFrom == OPEN_FROMMACRO) {
		Far::Panel pi(PANEL_ACTIVE);
		if (pi.is_ok()) {
			const PluginPanelItem * ppi = pi.get_current();
			PCWSTR fileName = ppi->FileName;
			LogNoise(L"ppi->FileName: '%s'\n", ppi->FileName);
			if (Base::Str::find(fileName, Base::PATH_SEPARATORS)) {
				Base::Str::copy(buf2, fileName, Base::lengthof(buf2));
			} else {
				Base::Str::copy(buf2, pi.get_current_directory(), Base::lengthof(buf2));
				if (!Base::Str::is_empty(buf2)) {
					Far::fsf().AddEndSlash(buf2);
				}
				Base::Str::cat(buf2, fileName, Base::lengthof(buf2));
			}
		}
	} else if (Info->OpenFrom == OPEN_COMMANDLINE) {
		OpenCommandLineInfo * info = (OpenCommandLineInfo*)Info->Data;
		LogNoise(L"comline: %s\n", info->CommandLine);
		Base::Str::copy(buf2, info->CommandLine, Base::lengthof(buf2));
		Far::fsf().Trim(buf2);
		Far::fsf().Unquote(buf2);
		Far::fsf().Trim(buf2);

		Base::Path::expand(buf1, Base::lengthof(buf1), buf2);
		LogNoise(L"buf: '%s'\n", buf1);

		if (Base::Path::is_relative(buf1)) {
			LogNoise(L"is relative\n");
			fsf().GetCurrentDirectoryW(Base::lengthof(buf2), buf2);
			if (!Base::Str::is_empty(buf2)) {
				Far::fsf().AddEndSlash(buf2);
			}
			Base::Str::cat(buf2, buf1, Base::lengthof(buf2));
		} else {
			LogNoise(L"is absolute\n");
			Base::Str::copy(buf2, buf1, Base::lengthof(buf2));
		}
	} else if (Info->OpenFrom == OPEN_VIEWER) {
		Far::Viewer::get_filename(buf2, Base::lengthof(buf2));
	}

	LogNoise(L"buf: '%s'\n", buf2);
	Far::fsf().Trim(buf2);
	Far::fsf().Unquote(buf2);
	Far::fsf().Trim(buf2);

	LogNoise(L"buf: '%s'\n", buf2);
	Base::Path::canonicalize(buf1, buf2);
	LogNoise(L"buf: '%s'\n", buf1);
	FileVersion fv(buf1);
	if (fv.is_ok()) {
		FVI fvi(fv);
//		wchar_t timeBuf[32] = {0};
//		_vsnwprintf(timeBuf, Base::lengthof(timeBuf), L"%S", ctime(&fv.created()));
		int i = 0, x = 70, y = 2;
		Far::InitDialogItemF Items[] = {
			{DI_TEXT, 5, y, 26, 0,          0,            (PCWSTR)MtxtFileFullVer},
			{DI_EDIT, 28, y++, x - 2, 0,    DIF_READONLY, fv.ver()},
			{DI_TEXT, 5, y, 26, 0,          0,            (PCWSTR)MtxtFileLang},
			{DI_EDIT, 28, y++, x - 2, 0,    DIF_READONLY, fv.lng()},
			{DI_TEXT, 5, y, 26, 0,          0,            (PCWSTR)fvi[i].msgTxt},
			{DI_EDIT, 28, y++, x - 2, 0,    DIF_READONLY, fvi[i++].data},
			{DI_TEXT, 5, y, 26, 0,          0,            (PCWSTR)fvi[i].msgTxt},
			{DI_EDIT, 28, y++, x - 2, 0,    DIF_READONLY, fvi[i++].data},
			{DI_TEXT, 5, y, 26, 0,          0,            (PCWSTR)MtxtMachine},
			{DI_EDIT, 28, y++, x - 2, 0,    DIF_READONLY, Base::NamedValues<WORD>::GetName(Machines, lengthof(Machines), fv.machine())},
//			{DI_TEXT, 5, y, 26, 0,          0,            (PCWSTR)MtxtCreated},
//			{DI_EDIT, 28, y++, x - 2, 0,    DIF_READONLY, timeBuf},
			{DI_TEXT, 5, y++, 0, 0,         DIF_SEPARATOR, L""},
			{DI_TEXT, 5, y, 26, 0,          0,            (PCWSTR)fvi[i].msgTxt},
			{DI_EDIT, 28, y++, x - 2, 0,    DIF_READONLY, fvi[i++].data},
			{DI_TEXT, 5, y, 26, 0,          0,            (PCWSTR)fvi[i].msgTxt},
			{DI_EDIT, 28, y++, x - 2, 0,    DIF_READONLY, fvi[i++].data},
			{DI_TEXT, 5, y, 26, 0,          0,            (PCWSTR)fvi[i].msgTxt},
			{DI_EDIT, 28, y++, x - 2, 0,    DIF_READONLY, fvi[i++].data},
			{DI_TEXT, 5, y, 26, 0,          0,            (PCWSTR)fvi[i].msgTxt},
			{DI_EDIT, 28, y++, x - 2, 0,    DIF_READONLY, fvi[i++].data},
			{DI_TEXT, 5, y, 26, 0,          0,            (PCWSTR)fvi[i].msgTxt},
			{DI_EDIT, 28, y++, x - 2, 0,    DIF_READONLY, fvi[i++].data},
			{DI_TEXT, 5, y, 26, 0,          0,            (PCWSTR)fvi[i].msgTxt},
			{DI_EDIT, 28, y++, x - 2, 0,    DIF_READONLY, fvi[i++].data},
			{DI_TEXT, 5, y, 26, 0,          0,            (PCWSTR)fvi[i].msgTxt},
			{DI_EDIT, 28, y++, x - 2, 0,    DIF_READONLY, fvi[i++].data},
			{DI_TEXT, 5, y, 26, 0,          0,            (PCWSTR)fvi[i].msgTxt},
			{DI_EDIT, 28, y++, x - 2, 0,    DIF_READONLY, fvi[i++].data},
			{DI_TEXT, 5, y, 26, 0,          0,            (PCWSTR)fvi[i].msgTxt},
			{DI_EDIT, 28, y++, x - 2, 0,    DIF_READONLY, fvi[i++].data},
			{DI_TEXT, 5, y, 26, 0,          0,            (PCWSTR)fvi[i].msgTxt},
			{DI_EDIT, 28, y++, x - 2, 0,    DIF_READONLY, fvi[i++].data},
			{DI_TEXT, 5, y++, 0, 0,         DIF_SEPARATOR, L""},
			{DI_BUTTON, 0, y++, 0, 0,       DIF_CENTERGROUP, (PCWSTR)Far::txtBtnOk},
			{DI_DOUBLEBOX, 3, 1, x, y,      0,               (PCWSTR)Far::DlgTitle},
		};

		size_t size = Base::lengthof(Items);
		FarDialogItem FarItems[size];
		InitDialogItemsF(Items, FarItems, size);
		HANDLE hndl = Far::psi().DialogInit(Far::get_plugin_guid(), &DialogGuid, -1, -1, x + 4, y + 2, L"Contents", FarItems, size, 0, 0, nullptr, 0);
		Far::psi().DialogRun(hndl);
		Far::psi().DialogFree(hndl);
	}
	return nullptr;
}

///=================================================================================================
Far::Plugin_i * create_FarPlugin(const PluginStartupInfo * psi)
{
	return new FarPlugin(psi);
}

void destroy(Far::Plugin_i * plugin)
{
	delete plugin;
}
