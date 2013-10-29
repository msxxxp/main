/**
	sortstr: Sort strings in editor
	FAR3 plugin

	c 2013 Andrew Grechkin

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

#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{

	//Date Version Types
	static const char DATE[] = "29";
	static const char MONTH[] = "10";
	static const char YEAR[] = "2013";
	static const char UBUNTU_VERSION_STYLE[] = "13.10";

	//Software Status
	static const char STATUS[] = "Beta";
	static const char STATUS_SHORT[] = "b";

	//Standard Version Type
	static const long MAJOR = 2;
	static const long MINOR = 0;
	static const long BUILD = 22;
	static const long REVISION = 0;

	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 22;
	#define RC_FILEVERSION 2,0,22,0
	#define RC_FILEVERSION_STRING "2, 0, 22, 0\0"
	static const char FULLVERSION_STRING[] = "2.0.22.0";

}
#endif //VERSION_H
