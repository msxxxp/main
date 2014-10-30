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

#include <basis/sys/cstr.hpp>
#include <basis/sys/logger.hpp>

namespace far3 {
	namespace dialog {

		struct ComboBoxBinding: public ItemBinding {
			ComboBoxBinding(ssize_t& value, FarListItem items[], size_t count);

			void save() const override;

			ssize_t get_width() const override;

			FarList* get_items() const;

		private:
			ssize_t& m_value;
			FarList  m_items;
		};

		ComboBoxBinding::ComboBoxBinding(ssize_t& value, FarListItem items[], size_t count) :
			m_value(value)
		{
			m_items.StructSize = sizeof(m_items);
			m_items.ItemsNumber = count;
			m_items.Items = items;
		}

		void ComboBoxBinding::save() const
		{
			m_value = psi().SendDlgMessage(get_dialog(), DM_LISTGETCURPOS, get_index(), 0);
			LogDebug(L"value: %Id\n", m_value);
		}

		ssize_t ComboBoxBinding::get_width() const
		{
			size_t width = 4;
			for (size_t i = 0; i < m_items.ItemsNumber; ++i) {
				width = simstd::max(width, cstr::length(m_items.Items[i].Text));
//				LogNoise(L"'%s'\n", m_items.Items[i].Text);
			}
			width = (width / 16 + 1) * 16 + 3;
			LogNoise(L"-> %Iu\n", width);
			return width;
		}

		FarList* ComboBoxBinding::get_items() const
		{
			return (FarList *)&m_items;
		}

		Item create_combobox(ssize_t& value, FarListItem items[], size_t count, FARDIALOGITEMFLAGS flags)
		{
			LogNoise(L"%Iu, %Id, 0x%I64X\n", count, value, flags);
			items[value].Flags |= LIF_SELECTED;
			auto binding = new ComboBoxBinding(value, items, count);
			Item ret(binding, DI_COMBOBOX, nullptr, flags);
			ret.ListItems = binding->get_items();
			ret.X2 = ret.X1 + ret.get_width() - 3;
			LogNoise(L"X1: %Id, Y1: %Id, X2: %Id\n", ret.X1, ret.Y1, ret.X2);

			return ret;
		}

	}
}
