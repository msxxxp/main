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

#include <far3/dialog.hpp>

namespace far3 {
	namespace dialog {

		HANDLE ItemBinding::get_dlg() const
		{
			return (this) ? *m_dlg : nullptr;
		}

		ssize_t ItemBinding::get_index() const
		{
			return (this) ? m_index : 0;
		}

		void ItemBinding::set_dlg(HANDLE * hndl)
		{
			if (this)
				m_dlg = hndl;
		}

		void ItemBinding::set_index(ssize_t index)
		{
			if (this)
				m_index = index;
		}

		void ItemBinding::save() const
		{
			if (this)
				save_();
		}

		ssize_t ItemBinding::get_height() const
		{
			return (this) ? get_height_() : 1;
		}

		ssize_t ItemBinding::get_width() const
		{
			return (this) ? get_width_() : 0;
		}

		ssize_t ItemBinding::get_height_() const
		{
			return 1;
		}

	}
}
