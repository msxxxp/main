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

		struct CheckBoxBinding: public ItemBinding {
			CheckBoxBinding(ssize_t& value, ssize_t min_width);

			void save() const override;

			ssize_t get_width() const override;

		private:
			ssize_t& m_value;
		};

		CheckBoxBinding::CheckBoxBinding(ssize_t& value, ssize_t min_width) :
			ItemBinding(min_width),
			m_value(value)
		{
		}

		void CheckBoxBinding::save() const
		{
			intptr_t Selected = psi().SendDlgMessage(get_dialog(), DM_GETCHECK, get_index(), 0);
			m_value = Selected;
			LogNoise(L"dlg: %p, index: %Id, value: %Id\n", get_dialog(), get_index(), m_value);
		}

		ssize_t CheckBoxBinding::get_width() const
		{
			return m_min_width;
		}

		Item create_checkbox(ssize_t& value, const wchar_t* text, ssize_t min_width, FARDIALOGITEMFLAGS flags)
		{
			LogNoise(L"'%s' %Id, 0x%I64X\n", text, value, flags);
			Item ret(new CheckBoxBinding(value, min_width), DI_CHECKBOX, text, flags);
			ret.Selected = value;

			return ret;
		}

	}
}
