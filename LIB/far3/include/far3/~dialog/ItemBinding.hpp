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

#ifndef _FAR3_DIALOG_ITEMBINDING_HPP_
#define _FAR3_DIALOG_ITEMBINDING_HPP_

#include <far3/~3rdparty/plugin.hpp>
//#include <far3/helper.hpp>

#include <basis/ext/pattern.hpp>

namespace far3 {
	namespace dialog {

		class ItemBinding {
		public:
			virtual ~ItemBinding() = default;

			HANDLE get_dlg() const;

			ssize_t get_index() const;

			void set_dlg(HANDLE * hndl);

			void set_index(ssize_t index);

			void save() const;

			ssize_t get_height() const;

			ssize_t get_width() const;

		protected:
			ItemBinding() :
				m_dlg(), m_index()
			{
			}

			ItemBinding(HANDLE * dlg, ssize_t index) :
				m_dlg(dlg), m_index(index)
			{
			}

		private:
			virtual void save_() const = 0;

			virtual ssize_t get_height_() const;

			virtual ssize_t get_width_() const = 0;

			HANDLE * m_dlg;
			ssize_t m_index;
		};

	}
}

#endif
