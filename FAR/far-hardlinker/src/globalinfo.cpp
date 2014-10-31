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
#include <guid.hpp>
#include <lang.hpp>

#include <far3/plugin.hpp>

#include <basis/sys/cstr.hpp>
#include <basis/sys/logger.hpp>

#include <version.h>

FarGlobalInfo::FarGlobalInfo()
{
	LogTraceObj();
	cstr::copy(prefix, L"hardlinker");
	m_cbOperation = 0;
	m_cbMask = 0;
	m_cbDoHardlink = 0;
	m_cbDoRecursive = 1;
	m_cbAskConfirmation = 1;
	m_cbEuristic = 0;
	m_cbRestrictionFileTime = 0;
	m_cbRestrictionFileAttributes = 0;
	m_cbFilterFileSize = 0;
	m_cbFilterFileReadOnly = 0;
	m_cbFilterFileHidden = 0;
	m_cbFilterFileSystem = 0;
	m_cbFilterFileLink = 0;
	m_cbFilterDirReadOnly = 0;
	m_cbFilterDirHidden = 0;
	m_cbFilterDirSystem = 0;
	m_cbFilterDirLink = 0;
	m_edMask = L"*";
}

FarGlobalInfo::~FarGlobalInfo()
{
	LogTraceObj();
}

const wchar_t* FarGlobalInfo::get_author() const
{
	LogTraceObj();
	return L"© 2014 Andrew Grechkin";
}

const wchar_t* FarGlobalInfo::get_description() const
{
	LogTraceObj();
	return L"Search duplicates and make hardlinks";
}

const GUID * FarGlobalInfo::get_guid() const
{
	LogTraceObj();
	return &PluginGuid;
}

const wchar_t* FarGlobalInfo::get_title() const
{
	LogTraceObj();
	return L"hardlinker";
}

VersionInfo FarGlobalInfo::get_version() const
{
	LogTraceObj();
	using namespace AutoVersion;
	return MAKEFARVERSION(MAJOR, MINOR, BUILD, FARMANAGERVERSION_BUILD, VS_RELEASE);
}

VersionInfo FarGlobalInfo::get_min_version() const
{
	LogTraceObj();
	return MAKEFARVERSION(3, 0, 0, 3000, VS_RELEASE);
}

far3::Plugin_i* FarGlobalInfo::CreatePlugin(const PluginStartupInfo* psi) const
{
	LogTrace();
	far3::Plugin_i* plugin = create_FarPlugin(psi);
	return plugin;
}

void FarGlobalInfo::load_settings()
{
	LogTrace();
}

void FarGlobalInfo::save_settings() const
{
	LogTrace();
}

FarGlobalInfo* get_global_info()
{
	return (FarGlobalInfo*)far3::helper_t::inst().get_global_info();
}
