﻿/**
 © 2013 Andrew Grechkin
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

#ifndef _LIBFAR_VIEWER_HPP_
#define _LIBFAR_VIEWER_HPP_

#include <libfar3/plugin.hpp>

namespace Far {

	namespace Viewer {

		ssize_t get_filename(PWSTR buf, ssize_t length);

	}

}

#endif
