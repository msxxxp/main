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

#include <far3/plugin.hpp>
//#include <far3/~plugin/GlobalInfo_i.hpp>
#include <far3/~plugin/PanelController_i.hpp>
//#include <far3/~plugin/Plugin_i.hpp>

#include <basis/sys/logger.hpp>

namespace far3 {

	///========================================================================================= nvi
	void PanelController_i::ClosePanelW(const ClosePanelInfo * info)
	{
		if (info->StructSize < sizeof(*info))
			return;
		Close(info);
	}

	ssize_t PanelController_i::CompareW(const CompareInfo * info)
	{
		if (info->StructSize < sizeof(*info))
			return -2;
		return Compare(info);
	}

	ssize_t PanelController_i::DeleteFilesW(const DeleteFilesInfo * info)
	{
		if (info->StructSize < sizeof(*info))
			return 0;
		return DeleteFiles(info);
	}

	void PanelController_i::FreeFindDataW(const FreeFindDataInfo * info)
	{
		if (info->StructSize < sizeof(*info))
			return;
		FreeFindData(info);
	}

	ssize_t PanelController_i::GetFilesW(GetFilesInfo * info)
	{
		info->StructSize = sizeof(*info);
		return GetFiles(info);
	}

	ssize_t PanelController_i::GetFindDataW(GetFindDataInfo * info)
	{
		info->StructSize = sizeof(*info);
		return GetFindData(info);
	}

	void PanelController_i::GetOpenPanelInfoW(OpenPanelInfo * info)
	{
		info->StructSize = sizeof(*info);
		GetOpenPanelInfo(info);
	}

	ssize_t PanelController_i::MakeDirectoryW(MakeDirectoryInfo * info)
	{
		info->StructSize = sizeof(*info);
		return MakeDirectory(info);
	}

	ssize_t PanelController_i::ProcessPanelEventW(const ProcessPanelEventInfo * info)
	{
		if (info->StructSize < sizeof(*info))
			return 0;
		return ProcessEvent(info);
	}

	ssize_t PanelController_i::ProcessHostFileW(const ProcessHostFileInfo * info)
	{
		if (info->StructSize < sizeof(*info))
			return 0;
		return ProcessHostFile(info);
	}

	ssize_t PanelController_i::ProcessPanelInputW(const ProcessPanelInputInfo * info)
	{
		if (info->StructSize < sizeof(*info))
			return 0;
		return ProcessInput(info);
	}

	ssize_t PanelController_i::PutFilesW(const PutFilesInfo * info)
	{
		if (info->StructSize < sizeof(*info))
			return 0;
		return PutFiles(info);
	}

	ssize_t PanelController_i::SetDirectoryW(const SetDirectoryInfo * info)
	{
		if (info->StructSize < sizeof(*info))
			return 0;
		return SetDirectory(info);
	}

	ssize_t PanelController_i::SetFindListW(const SetFindListInfo * info)
	{
		if (info->StructSize < sizeof(*info))
			return 0;
		return SetFindList(info);
	}

	intptr_t PanelController_i::update(bool keep_selection) const
	{
		return psi().PanelControl((HANDLE)this, FCTL_UPDATEPANEL, keep_selection, nullptr);
	}

	intptr_t PanelController_i::redraw() const
	{
		return psi().PanelControl((HANDLE)this, FCTL_REDRAWPANEL, 0, nullptr);
	}

	///============================================================================================
	void PanelController_i::Close(const ClosePanelInfo * /*info*/)
	{
		delete this;
	}

	ssize_t PanelController_i::Compare(const CompareInfo * /*info*/)
	{
		return -2;
	}

	ssize_t PanelController_i::DeleteFiles(const DeleteFilesInfo * /*info*/)
	{
		return 0;
	}

	void PanelController_i::FreeFindData(const FreeFindDataInfo * /*info*/)
	{
	}

	ssize_t PanelController_i::GetFiles(GetFilesInfo * /*info*/)
	{
		return 0;
	}

	ssize_t PanelController_i::GetFindData(GetFindDataInfo * /*info*/)
	{
		return 0;
	}

	ssize_t PanelController_i::MakeDirectory(MakeDirectoryInfo * /*info*/)
	{
		return 0;
	}

	ssize_t PanelController_i::ProcessEvent(const ProcessPanelEventInfo * /*info*/)
	{
		return 0;
	}

	ssize_t PanelController_i::ProcessInput(const ProcessPanelInputInfo * /*info*/)
	{
		return 0;
	}

	ssize_t PanelController_i::ProcessHostFile(const ProcessHostFileInfo * /*info*/)
	{
		return 0;
	}

	ssize_t PanelController_i::PutFiles(const PutFilesInfo * /*info*/)
	{
		return 0;
	}

	ssize_t PanelController_i::SetDirectory(const SetDirectoryInfo * /*info*/)
	{
		return 0;
	}

	ssize_t PanelController_i::SetFindList(const SetFindListInfo * /*info*/)
	{
		return 0;
	}
}
