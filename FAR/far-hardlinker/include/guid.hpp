/**
	hardlinker: Search duplicates and make hardlinks
	FAR3-lua plugin

	© 2014 Andrew Grechkin

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
**/

#ifndef _GUID_HPP_
#define _GUID_HPP_

#include <windows.h>

// {5ACBC89C-5B6E-409A-B1D5-3A323579A140}
DEFINE_GUID(PluginGuid,
0x5acbc89c, 0x5b6e, 0x409a, 0xb1, 0xd5, 0x3a, 0x32, 0x35, 0x79, 0xa1, 0x40);

// {C7DFC9FD-C177-4152-B2F5-1A48FAB6F715}
DEFINE_GUID(MenuGuid,
0xc7dfc9fd, 0xc177, 0x4152, 0xb2, 0xf5, 0x1a, 0x48, 0xfa, 0xb6, 0xf7, 0x15);

// {C62B25B8-137A-4119-9706-FFEFA1287072}
DEFINE_GUID(DialogGuid,
0xc62b25b8, 0x137a, 0x4119, 0x97, 0x6, 0xff, 0xef, 0xa1, 0x28, 0x70, 0x72);

// {C62B25B8-137A-4119-9706-FFEFA1287072}
DEFINE_GUID(ListMenuGuid,
0xc62b25b8, 0x137a, 0x4119, 0x97, 0x6, 0xff, 0xef, 0xa1, 0x28, 0x70, 0x72);


#endif
