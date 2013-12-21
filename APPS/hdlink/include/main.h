/**
	hdlink console tool
	Search duplicate files and make hardlinks

	© 2010 Andrew Grechkin

	Some code was adopted from:
	DFHL - Duplicate File Hard Linker, a small tool to gather some space
	from duplicate files on your hard disk
	Copyright (C) 2004, 2005 Jens Scheffler & Oliver Schneider
	http://www.jensscheffler.de/dfhl

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
#ifndef __HARDLINKER__HPP
#define __HARDLINKER__HPP

#include <libwin_def/win_def.h>

const		size_t	FirstBlock = 65535;
void		logError(DWORD errNumber, PCWSTR message, ...);

///==================================================================================== WinCryptHash

///============================================================================================ Path
///======================================================================================== FileHash
///============================================================================================ File
bool		operator==(const Shared_ptr<File> &lhs, const Shared_ptr<File> &rhs) {
	if (lhs->size() != rhs->size())
		return	false;
	if (lhs->inode() == rhs->inode())
		return	true;
	return	false;
}
bool		isSameVolume(const Shared_ptr<File> &lhs, const Shared_ptr<File> &rhs) {
	return	lhs->inode().vol_sn() == rhs->inode().vol_sn();
}

bool		CompareBySize(const Shared_ptr<File> &f1, const Shared_ptr<File> &f2) {
	if (f1->size() < f2->size())
		return	true;
	return	false;
}
bool		CompareBySizeAndTime(const Shared_ptr<File> &f1, const Shared_ptr<File> &f2) {
	if (f1->size() < f2->size())
		return	true;
	if (f1->size() == f2->size())
		return	f1->time() < f2->time();
	return	false;
}

#endif
