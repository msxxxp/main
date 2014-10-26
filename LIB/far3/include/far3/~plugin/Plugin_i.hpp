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

#ifndef _FAR3_PLUGIN_I_HPP_
#define _FAR3_PLUGIN_I_HPP_

#include <far3/plugin.hpp>

#include <basis/ext/pattern.hpp>

namespace far3 {

	class Plugin_i: public pattern::Destroyable {
	public:
		virtual ~Plugin_i() = default;

		Plugin_i(const PluginStartupInfo * info);

	public:
		void GetPluginInfoW(PluginInfo * info);

		PanelController_i * OpenW(const OpenInfo * info);

		void ExitFARW(const ExitInfo * info);

		const PluginStartupInfo & psi() const
		{
			return m_psi;
		}

		const FarStandardFunctions & fsf() const
		{
			return m_fsf;
		}

	private:
		virtual void GetPluginInfo(PluginInfo * pi) = 0;

		virtual PanelController_i * Open(const OpenInfo * info);

		virtual void ExitFAR(const ExitInfo * info);

	private:
		PluginStartupInfo    m_psi;
		FarStandardFunctions m_fsf;
	};

}

#endif
