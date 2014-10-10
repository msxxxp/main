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
#include <basis/simstd/string>

namespace Far {

	struct PluginEditFieldBinding: public DialogItemBinding_i {
		PluginEditFieldBinding(simstd::wstring * value);

		void save_() const override;

		ssize_t get_width_() const override;

	private:
		simstd::wstring * m_value;
	};

	PluginEditFieldBinding::PluginEditFieldBinding(simstd::wstring * value) :
		m_value(value)
	{
		LogTrace();
	}

	void PluginEditFieldBinding::save_() const
	{
		auto DataPtr = (const wchar_t *)psi().SendDlgMessage(get_dlg(), DM_GETCONSTTEXTPTR, get_index(), nullptr);
		*m_value = (DataPtr) ? DataPtr : EMPTY_STR;
		LogDebug(L"value: '%s'\n", m_value->c_str());
	}

	ssize_t PluginEditFieldBinding::get_width_() const
	{
		return static_cast<ssize_t>(m_value->size());
	}

	FarDialogItem_t * create_edit(simstd::wstring * value, ssize_t width, PCWSTR history_id, bool use_last_history, FARDIALOGITEMFLAGS flags)
	{
		LogNoise(L"'%s', flags: 0x%I64X\n", value->c_str(), flags);
		auto ret = new FarDialogItem_t(new PluginEditFieldBinding(value), DI_EDIT, value->c_str(), flags);
		if (width == -1)
			width = 10;

		ret->X2 = ret->X1 + width - 1;

		if (history_id) {
			ret->History = history_id;
			ret->Flags |= DIF_HISTORY;
			if (use_last_history)
				ret->Flags |= DIF_USELASTHISTORY;
		}

		return ret;
	}

	FarDialogItem_t * create_password(simstd::wstring * value, ssize_t width, FARDIALOGITEMFLAGS flags)
	{
		LogNoise(L"%flags: 0x%I64X\n", flags);
		auto ret = new FarDialogItem_t(new PluginEditFieldBinding(value), DI_PSWEDIT, value->c_str(), flags);
		if (width == -1)
			width = 10;

		ret->X2 = ret->X1 + width - 1;

		return ret;
	}
}
