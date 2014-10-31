
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

#include <far3/dialog.hpp>

#include <basis/ext/pattern.hpp>

namespace far3 {
	namespace dialog {

		class ItemBinding {
		public:
			virtual ~ItemBinding() = default;

			ItemBinding(ssize_t min_width);

			HANDLE get_dialog() const;

			ssize_t get_index() const;

			void set_dialog(HANDLE dialog);

			void set_index(ssize_t index);

			virtual void save() const;

			virtual ssize_t get_width() const;

			virtual ssize_t get_height() const;

		protected:
			ItemBinding(HANDLE dialog, ssize_t index, ssize_t min_width);

		private:
			HANDLE  m_dialog;
			ssize_t m_index;
			ssize_t m_min_width;
		};

	}
}

#endif
