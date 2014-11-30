
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

#include <far3/message.hpp>

namespace far3 {
	namespace message {

		void ibox(PCWSTR text, PCWSTR tit)
		{
			PCWSTR Msg[] = {tit, text};
			psi().Message(get_plugin_guid(), nullptr, FMSG_MB_OK, nullptr, Msg, lengthof(Msg), 1);
		}

		void mbox(PCWSTR text, PCWSTR tit)
		{
			PCWSTR Msg[] = {tit, text};
			psi().Message(get_plugin_guid(), nullptr, FMSG_MB_OK, nullptr, Msg, lengthof(Msg), 1);
		}

		void ebox(PCWSTR text, PCWSTR tit)
		{
			PCWSTR Msg[] = {tit, text};
			psi().Message(get_plugin_guid(), nullptr, FMSG_WARNING | FMSG_MB_OK, nullptr, Msg, lengthof(Msg), 1);
		}

		void ebox(PCWSTR msgs[], size_t size, PCWSTR help)
		{
			psi().Message(get_plugin_guid(), nullptr, FMSG_WARNING | FMSG_MB_OK, help, msgs, size, 1);
		}

//		void ebox(const cstr::mstring& msg, PCWSTR title)
//		{
//			size_t len = msg.size() + 1;
//			PCWSTR tmp[len];
//			tmp[0] = title;
//			for (size_t i = 0; i < msg.size(); ++i) {
//				tmp[i + 1] = msg[i];
//			}
//			psi().Message(get_plugin_guid(), nullptr, FMSG_WARNING | FMSG_MB_OK, nullptr, tmp, len, 1);
//		}
//
		void ebox(DWORD err)
		{
//		ustring title(L"Error: ");
//		title += Base::as_str(err);
			::SetLastError(err);
//		PCWSTR Msg[] = {title.c_str(), L"OK", };
			psi().Message(get_plugin_guid(), nullptr, FMSG_WARNING | FMSG_ERRORTYPE | FMSG_MB_OK, nullptr, nullptr, 0, 0);
		}

		bool question(PCWSTR text, PCWSTR tit)
		{
			PCWSTR Msg[] = {tit, text};
			return psi().Message(get_plugin_guid(), nullptr, FMSG_WARNING | FMSG_MB_OKCANCEL, nullptr, Msg, lengthof(Msg), 2) == 0;
		}

	}
}
