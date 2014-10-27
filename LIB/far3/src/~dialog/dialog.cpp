
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

namespace far3 {
	namespace dialog {

		struct FacadeImpl: public Facade_i {
			~FacadeImpl();

			FacadeImpl(const GUID & guid, int x1, int y1, int x2, int y2, FarDialogItem* items, int count, DWORD flags, FARWINDOWPROC DlgProc);

			bool is_valid() const;

			ssize_t show() override;

			int Check(int index) const;

			bool IsChanged(int index) const;

			PCWSTR Str(int index) const;

			DWORD Flags(int index);

			DWORD Type(int index);

			ssize_t get_list_position(int index) const;

		private:
			HANDLE m_hndl;
		};

		FacadeImpl::~FacadeImpl()
		{
			if (is_valid()) {
				psi().DialogFree(m_hndl);
			}
		}

		FacadeImpl::FacadeImpl(const GUID & dguid, int X1, int Y1, int X2, int Y2, FarDialogItem* Item, int ItemsNumber, DWORD Flags, FARWINDOWPROC DlgProc) :
			m_hndl(nullptr)
		{
			m_hndl = psi().DialogInit(get_plugin_guid(), &dguid, X1, Y1, X2, Y2, nullptr, Item, ItemsNumber, 0, Flags, DlgProc, 0);
		}

		inline bool FacadeImpl::is_valid() const
		{
			return m_hndl && m_hndl != INVALID_HANDLE_VALUE;
		}

		ssize_t FacadeImpl::show()
		{
			if (is_valid())
				return psi().DialogRun(m_hndl);
			return -1;
		}

		int FacadeImpl::Check(int index) const
		{
			return (int)psi().SendDlgMessage(m_hndl, DM_GETCHECK, index, 0);
		}

		bool FacadeImpl::IsChanged(int index) const
		{
			return !(bool)psi().SendDlgMessage(m_hndl, DM_EDITUNCHANGEDFLAG, index, nullptr);
		}

		PCWSTR FacadeImpl::Str(int index) const
		{
			return (PCWSTR)psi().SendDlgMessage(m_hndl, DM_GETCONSTTEXTPTR, index, nullptr);
		}

		DWORD FacadeImpl::Flags(int index)
		{
			FarDialogItem DialogItem;
			return psi().SendDlgMessage(m_hndl, DM_GETDLGITEMSHORT, index, &DialogItem) ? DialogItem.Flags : 0;
		}

		DWORD FacadeImpl::Type(int index)
		{
			FarDialogItem DialogItem;
			return psi().SendDlgMessage(m_hndl, DM_GETDLGITEMSHORT, index, &DialogItem) ? DialogItem.Type : 0;
		}

		ssize_t FacadeImpl::get_list_position(int index) const
		{
			return psi().SendDlgMessage(m_hndl, DM_LISTGETCURPOS, index, 0);
		}

		inline PCWSTR get_data_ptr(HANDLE m_hndl, size_t in)
		{
			return (PCWSTR)psi().SendDlgMessage(m_hndl, DM_GETCONSTTEXTPTR, in, 0);
		}

		inline bool get_checkbox(HANDLE m_hndl, size_t in)
		{
			return (bool)psi().SendDlgMessage(m_hndl, DM_GETCHECK, in, 0);
		}

		///=============================================================================================================
		Facade create(const GUID & guid, int x1, int y1, int x2, int y2, FarDialogItem* items, int count, DWORD flags)
		{
			simstd::unique_ptr<FacadeImpl> tmp(new FacadeImpl(guid, x1, y1, x2, y2, items, count, flags, nullptr));
			return tmp->is_valid() ? Facade(simstd::move(tmp)) : Facade();
		}

	}
}
