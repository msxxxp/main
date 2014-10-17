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

#ifndef _LIBFAR3_DIALOG_HPP_
#define _LIBFAR3_DIALOG_HPP_

#include <libfar3/plugin.hpp>
#include <libfar3/helper.hpp>

#include <basis/simstd/memory>

namespace Far {

	struct Dialog_i {
		virtual ~Dialog_i() = default;

		virtual ssize_t show();
	};

	typedef simstd::unique_ptr<Dialog_i> Dialog;

	Dialog create_dialog(const GUID & guid, int x1, int y1, int x2, int y2, FarDialogItem* items, int count, DWORD flags);

}

#endif
