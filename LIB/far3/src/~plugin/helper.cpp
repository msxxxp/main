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

#include <far3/plugin.hpp>

#include <far3/~plugin/GlobalInfo_i.hpp>
#include <far3/~plugin/Plugin_i.hpp>

namespace far3 {

	helper_t& helper_t::inst()
	{
		static helper_t ret;
		return ret;
	}

	helper_t::~helper_t()
	{
		delete m_gi;
	}

	helper_t& helper_t::init(GlobalInfo_i * gi)
	{
		m_gi = gi;
		return *this;
	}

	const GUID* helper_t::get_guid() const
	{
		return m_gi->get_guid();
	}

	const PluginStartupInfo& helper_t::psi() const
	{
		return get_plugin()->psi();
	}

	const FarStandardFunctions& helper_t::fsf() const
	{
		return get_plugin()->fsf();
	}

	GlobalInfo_i* helper_t::get_global_info() const
	{
		return m_gi;
	}

	Plugin_i* helper_t::get_plugin() const
	{
		return m_gi->get_plugin();
	}

	helper_t::helper_t() :
		m_gi(nullptr)
	{
	}

}
