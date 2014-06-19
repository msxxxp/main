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

#include <libfar3/dialog_builder.hpp>
#include <libfar3/helper.hpp>

#include <basis/sys/logger.hpp>

namespace Far {

	struct CheckBoxBinding: public DialogItemBinding_i {
		CheckBoxBinding(ssize_t * value);

		void save_() const override;

		ssize_t get_width_() const override;

	private:
		ssize_t * Value;
	};

	CheckBoxBinding::CheckBoxBinding(ssize_t * value) :
		Value(value)
	{
	}

	void CheckBoxBinding::save_() const
	{
		intptr_t Selected = psi().SendDlgMessage(get_dlg(), DM_GETCHECK, get_index(), 0);
		*Value = Selected;
		LogNoise(L"dlg: %p, index: %Id, value: %Id\n", get_dlg(), get_index(), *Value);
	}

	ssize_t CheckBoxBinding::get_width_() const
	{
		return 0;
	}

	FarDialogItem_t * create_checkbox(ssize_t * value, PCWSTR text, FARDIALOGITEMFLAGS flags)
	{
		LogNoise(L"'%s' %Id, 0x%I64X\n", text, *value, flags);
		auto ret = new FarDialogItem_t(new CheckBoxBinding(value), DI_CHECKBOX, text, flags);
		ret->Selected = *value;

		return ret;
	}

}
