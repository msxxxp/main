#include <libfar3/dialog.hpp>

namespace Far {

	struct DialogImpl: public Dialog_i {
		~DialogImpl();

		DialogImpl(const GUID & guid, int x1, int y1, int x2, int y2, FarDialogItem* items, int count, DWORD flags, FARWINDOWPROC DlgProc);

		bool is_valid() const;

		ssize_t show() override;

		int Check(int index) const
		{
			return (int)psi().SendDlgMessage(m_hndl, DM_GETCHECK, index, 0);
		}

		bool IsChanged(int index) const
		{
			return !(bool)psi().SendDlgMessage(m_hndl, DM_EDITUNCHANGEDFLAG, index, nullptr);
		}

		PCWSTR Str(int index) const
		{
			return (PCWSTR)psi().SendDlgMessage(m_hndl, DM_GETCONSTTEXTPTR, index, nullptr);
		}

		DWORD Flags(int index)
		{
			FarDialogItem DialogItem;
			return psi().SendDlgMessage(m_hndl, DM_GETDLGITEMSHORT, index, &DialogItem) ? DialogItem.Flags : 0;
		}

		DWORD Type(int index)
		{
			FarDialogItem DialogItem;
			return psi().SendDlgMessage(m_hndl, DM_GETDLGITEMSHORT, index, &DialogItem) ? DialogItem.Type : 0;
		}

		ssize_t get_list_position(int index) const
		{
			return psi().SendDlgMessage(m_hndl, DM_LISTGETCURPOS, index, 0);
		}

	private:
		HANDLE m_hndl;
	};

	DialogImpl::~DialogImpl()
	{
		if (is_valid()) {
			psi().DialogFree(m_hndl);
		}
	}

	DialogImpl::DialogImpl(const GUID & dguid, int X1, int Y1, int X2, int Y2, FarDialogItem* Item, int ItemsNumber, DWORD Flags = 0, FARWINDOWPROC DlgProc) :
		m_hndl(nullptr)
	{
		m_hndl = psi().DialogInit(get_plugin_guid(), &dguid, X1, Y1, X2, Y2, nullptr, Item, ItemsNumber, 0, Flags, DlgProc, 0);
	}

	inline bool DialogImpl::is_valid() const
	{
		return m_hndl && m_hndl != INVALID_HANDLE_VALUE;
	}

	ssize_t DialogImpl::show()
	{
		if (is_valid())
			return psi().DialogRun(m_hndl);
		return -1;
	}

	int DialogImpl::Check(int index) const
	{
		return (int)psi().SendDlgMessage(m_hndl, DM_GETCHECK, index, 0);
	}

	bool DialogImpl::IsChanged(int index) const
	{
		return !(bool)psi().SendDlgMessage(m_hndl, DM_EDITUNCHANGEDFLAG, index, nullptr);
	}

	PCWSTR DialogImpl::Str(int index) const
	{
		return (PCWSTR)psi().SendDlgMessage(m_hndl, DM_GETCONSTTEXTPTR, index, nullptr);
	}

	DWORD DialogImpl::Flags(int index)
	{
		FarDialogItem DialogItem;
		return psi().SendDlgMessage(m_hndl, DM_GETDLGITEMSHORT, index, &DialogItem) ? DialogItem.Flags : 0;
	}

	DWORD DialogImpl::Type(int index)
	{
		FarDialogItem DialogItem;
		return psi().SendDlgMessage(m_hndl, DM_GETDLGITEMSHORT, index, &DialogItem) ? DialogItem.Type : 0;
	}

	ssize_t DialogImpl::get_list_position(int index) const
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
	Dialog create_dialog(const GUID & guid, int x1, int y1, int x2, int y2, FarDialogItem* items, int count, DWORD flags)
	{
		simstd::unique_ptr<DialogImpl> tmp(new DialogImpl(guid, x1, y1, x2, y2, items, count, flags));
		return tmp->is_valid() ? Dialog(simstd::move(tmp)) : Dialog();
	}
}
