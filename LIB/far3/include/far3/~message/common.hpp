
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

#ifndef _FAR3_MESSAGE_COMMON_HPP_
#define _FAR3_MESSAGE_COMMON_HPP_

#include <far3/message.hpp>

namespace far3 {
	namespace message {

		enum {
			Empty        = -1,
			MenuTitle    = 0,
			DiskTitle    = 1,
			DlgTitle     = 2,
			txtBtnOk     = 3,
			txtBtnCancel = 4,
		};

		const wchar_t* get(ssize_t msgId);

	}
}

#endif
