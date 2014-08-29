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

#ifndef _LANG_HPP_
#define _LANG_HPP_

enum FarMessage {
	txOperation = 5,
	lbSearchOnly,
	lbHardlinkAuto,
	lbHardlinkManual,
	cbMask,
	cbDoHardlink,
	cbDoRecursive,
	cbAskConfirmation,
	cbEuristic,
	txDirFilters,
	txFileFilters,
	txFileRestrictions,
	cbRestrictionFileTime,
	cbRestrictionFileAttributes,
	cbFilterFileSize,
	cbFilterFileReadOnly,
	cbFilterFileHidden,
	cbFilterFileSystem,
	cbFilterFileLink,
	cbFilterDirReadOnly,
	cbFilterDirHidden,
	cbFilterDirSystem,
	cbFilterDirLink,
};

#endif
