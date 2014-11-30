
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

#ifndef _FAR3_MESSAGE_BOX_HPP_
#define _FAR3_MESSAGE_BOX_HPP_

#include <far3/message.hpp>

namespace far3 {
	namespace message {

		void ibox(const wchar_t* text, const wchar_t* tit = L"Info");

		void mbox(const wchar_t* text, const wchar_t* tit = L"Message");

		void ebox(const wchar_t* text, const wchar_t* tit = L"Error");

		void ebox(const wchar_t* msgs[], size_t size, const wchar_t* help = nullptr);

//		void ebox(const cstr::mstring& msg, const wchar_t* title = EMPTY_STR);

		void ebox(DWORD err);

		bool question(const wchar_t* text, const wchar_t* tit);

	}
}

#endif
