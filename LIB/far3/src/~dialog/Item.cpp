
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

#include <basis/simstd/algorithm>

namespace far3 {
	namespace dialog {

		Item::~Item()
		{
			delete reinterpret_cast<ItemBinding*>(UserData);
		}

		Item::Item(ssize_t min_width, FARDIALOGITEMTYPES type, PCWSTR text, FARDIALOGITEMFLAGS flags)
		{
			LogNoise(L"'%s' %d, 0x%I64X\n", text, type, flags);
			memory::zero(*this);
			Type = type;
			Data = text;
			Flags = flags;
			UserData = reinterpret_cast<intptr_t>(new ItemBinding(min_width));
		}

		Item::Item(ItemBinding* binding, FARDIALOGITEMTYPES type, PCWSTR text, FARDIALOGITEMFLAGS flags)
		{
			LogNoise(L"'%s' %d, 0x%I64X, %p\n", text, type, flags, binding);
			memory::zero(*this);
			Type = type;
			Data = text;
			Flags = flags;
			UserData = reinterpret_cast<intptr_t>(binding);
		}

		Item::Item(Item&& other) :
			FarDialogItem(other)
		{
			LogTrace();
			other.UserData = 0;
		}

		Item& Item::operator =(Item&& other)
		{
			LogTrace();
			FarDialogItem::operator =(other);
//			::memcpy(this, &other, sizeof(*this));
			other.UserData = 0;
			return *this;
		}

		ssize_t Item::get_height() const
		{
			LogTrace();
			return reinterpret_cast<ItemBinding*>(UserData)->get_height();
		}

		ssize_t Item::get_width() const
		{
			LogTrace();
			ssize_t ret = 0;
			switch (Type) {
				case DI_TEXT:
					ret = get_text_width();
					break;

				case DI_DOUBLEBOX:
					ret = simstd::max(get_text_width(), static_cast<ssize_t>(22));
					break;

				case DI_CHECKBOX:
					ret = 4 + simstd::max(get_text_width(), reinterpret_cast<ItemBinding*>(UserData)->get_width());
					break;
				case DI_RADIOBUTTON:
				case DI_BUTTON:
					ret = get_text_width() + 4;
					break;

				case DI_EDIT:
				case DI_FIXEDIT:
				case DI_PSWEDIT: {
					int Width = X2 - X1 + 1;
					if (Flags & DIF_HISTORY)
						Width++;
					ret = Width;
					break;
				}

				case DI_SINGLEBOX:
					ret = X2 - X1 + 1;
					break;

				default:
					ret = reinterpret_cast<ItemBinding*>(UserData)->get_width();
					break;
			}
			LogNoise(L"-> %Id\n", ret);
			return ret;
		}

		ssize_t Item::get_text_width() const
		{
			return Data ? cstr::length(Data) : 0;
		}

		void Item::set_dialog(HANDLE dialog)
		{
			reinterpret_cast<ItemBinding*>(UserData)->set_dialog(dialog);
		}

		void Item::set_index(ssize_t index)
		{
			reinterpret_cast<ItemBinding*>(UserData)->set_index(index);
		}

		void Item::set_dimensions(ssize_t x, ssize_t y, ssize_t width, ssize_t height)
		{
			X1 = x;
			Y1 = y;
			X2 = simstd::max(x + width - 1, static_cast<ssize_t>(0));
			Y2 = simstd::max(y + height - 1, static_cast<ssize_t>(0));
			LogNoise(L"(%Id, %Id, %Id, %Id)\n", X1, Y1, X2, Y2);
		}

		void Item::save() const
		{
			reinterpret_cast<ItemBinding*>(UserData)->save();
		}

	}
}
