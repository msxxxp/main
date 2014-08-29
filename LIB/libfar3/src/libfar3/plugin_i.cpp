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

#include <libfar3/plugin_i.hpp>
#include <libfar3/helper.hpp>

#include <basis/sys/logger.hpp>

namespace Far {

	///========================================================================================= nvi
	void Plugin_i::GetPluginInfoW(PluginInfo * info)
	{
		info->StructSize = sizeof(*info);
		GetPluginInfo(info);
	}

	PanelController_i * Plugin_i::OpenW(const OpenInfo * info)
	{
		LogTrace();
		if (info->StructSize < sizeof(*info))
			return nullptr;
		return Open(info);
	}

	void Plugin_i::ExitFARW(const ExitInfo * info)
	{
		LogTrace();
		if (info->StructSize < sizeof(*info))
			return;
		return ExitFAR(info);
	}

	///=============================================================================================
	Plugin_i::Plugin_i(const PluginStartupInfo * info)
	{
		m_psi = *info;
		m_fsf = *info->FSF;
		m_psi.FSF = &m_fsf;
	}

	///=============================================================================================
	PanelController_i * Plugin_i::Open(const OpenInfo * info)
	{
		LogTrace();
		UNUSED(info);
		return nullptr;
	}

	void Plugin_i::ExitFAR(const ExitInfo * info)
	{
		LogTrace();
		UNUSED(info);
	}

}
