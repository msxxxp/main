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

#include <libfar3/globalinfo_i.hpp>
#include <libfar3/plugin_i.hpp>

#include <basis/sys/logger.hpp>

namespace Far {

	///=============================================================================================
	void GlobalInfo_i::GetGlobalInfoW(GlobalInfo * info) const
	{
		LogTrace();
		info->StructSize = sizeof(*info);
		info->MinFarVersion = get_min_version();
		info->Version = get_version();
		info->Author = get_author();
		info->Description = get_description();
		info->Guid = *get_guid();
		info->Title = get_title();
	}

	intptr_t GlobalInfo_i::ConfigureW(const ConfigureInfo * info)
	{
		LogTrace();
		if (info->StructSize < sizeof(*info))
			return 0;
		return Configure(info);
	}

	void GlobalInfo_i::SetStartupInfoW(const PluginStartupInfo * info)
	{
		LogTrace();
		if (info->StructSize < sizeof(*info))
			return;
		if (!m_plugin)
			m_plugin = CreatePlugin(info);
	}

	Plugin_i * GlobalInfo_i::get_plugin() const
	{
		return m_plugin;
	}

	///=============================================================================================
	GlobalInfo_i::GlobalInfo_i() :
		m_plugin(nullptr)
	{
		LogTrace();
	}

	GlobalInfo_i::~GlobalInfo_i()
	{
		delete m_plugin;
		LogTrace();
	}

	VersionInfo GlobalInfo_i::get_min_version() const
	{
		return FARMANAGERVERSION;
	}

	intptr_t GlobalInfo_i::Configure(const ConfigureInfo * /*info*/)
	{
		LogTrace();
		return 0;
	}

}
