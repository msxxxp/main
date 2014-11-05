
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

		ItemBinding::ItemBinding(ssize_t min_width) :
			m_dialog(),
			m_index(),
			m_min_width(min_width)
		{
		}

		ItemBinding::ItemBinding(HANDLE dlg, ssize_t index, ssize_t min_width) :
			m_dialog(dlg),
			m_index(index),
			m_min_width(min_width)
		{
		}

		HANDLE ItemBinding::get_dialog() const
		{
			return m_dialog;
		}

		ssize_t ItemBinding::get_index() const
		{
			return m_index;
		}

		void ItemBinding::set_dialog(HANDLE dialog)
		{
			m_dialog = dialog;
		}

		void ItemBinding::set_index(ssize_t index)
		{
			m_index = index;
		}

		void ItemBinding::save() const
		{
		}

		ssize_t ItemBinding::get_width() const
		{
			return m_min_width;
		}

		ssize_t ItemBinding::get_height() const
		{
			return 1;
		}

	}
}
