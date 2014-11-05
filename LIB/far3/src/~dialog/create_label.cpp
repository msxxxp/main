
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

#include <basis/sys/logger.hpp>

namespace far3 {
	namespace dialog {

		Item create_label(const wchar_t* text, ssize_t min_width, FARDIALOGITEMFLAGS flags)
		{
			LogNoise(L"'%s' %Id, 0x%I64X\n", text, min_width, flags);
			return Item(min_width, DI_TEXT, text, flags);
		}

	}
}
