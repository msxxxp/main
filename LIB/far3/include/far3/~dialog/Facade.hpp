
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

#ifndef _FAR3_DIALOG_FACADE_HPP_
#define _FAR3_DIALOG_FACADE_HPP_

#include <far3/dialog.hpp>

namespace far3 {
	namespace dialog {

		struct Facade_i {
			virtual ~Facade_i() = default;

			virtual ssize_t show() = 0;
		};

		typedef simstd::unique_ptr<Facade_i> Facade;

		Facade create(const GUID & guid, int x1, int y1, int x2, int y2, FarDialogItem* items, int count, DWORD flags);

	}
}

#endif
