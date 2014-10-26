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

#ifndef _FAR3_PANELCONTROLLER_I_HPP_
#define _FAR3_PANELCONTROLLER_I_HPP_

#include <far3/plugin.hpp>

namespace far3 {

	class PanelController_i {
	public:
		virtual ~PanelController_i() = default;

		PanelController_i() = default;

		void ClosePanelW(const ClosePanelInfo * info);

		ssize_t CompareW(const CompareInfo * info);

		ssize_t DeleteFilesW(const DeleteFilesInfo * info);

		void FreeFindDataW(const FreeFindDataInfo * info);

		ssize_t GetFilesW(GetFilesInfo * info);

		ssize_t GetFindDataW(GetFindDataInfo * info);

		void GetOpenPanelInfoW(OpenPanelInfo * info);

		ssize_t MakeDirectoryW(MakeDirectoryInfo * info);

		ssize_t ProcessPanelEventW(const ProcessPanelEventInfo * info);

		ssize_t ProcessHostFileW(const ProcessHostFileInfo * info);

		ssize_t ProcessPanelInputW(const ProcessPanelInputInfo * info);

		ssize_t PutFilesW(const PutFilesInfo * info);

		ssize_t SetDirectoryW(const SetDirectoryInfo * info);

		ssize_t SetFindListW(const SetFindListInfo * info);

		intptr_t update(bool keep_selection = true) const;

		intptr_t redraw() const;

	private:
		virtual void Close(const ClosePanelInfo * info);

		virtual ssize_t Compare(const CompareInfo * info);

		virtual ssize_t DeleteFiles(const DeleteFilesInfo * info);

		virtual void FreeFindData(const FreeFindDataInfo * info);

		virtual ssize_t GetFiles(GetFilesInfo * info);

		virtual ssize_t GetFindData(GetFindDataInfo * info);

		virtual void GetOpenPanelInfo(OpenPanelInfo * info) = 0;

		virtual ssize_t MakeDirectory(MakeDirectoryInfo * info);

		virtual ssize_t ProcessEvent(const ProcessPanelEventInfo * info);

		virtual ssize_t ProcessInput(const ProcessPanelInputInfo * info);

		virtual ssize_t ProcessHostFile(const ProcessHostFileInfo * info);

		virtual ssize_t PutFiles(const PutFilesInfo * info);

		virtual ssize_t SetDirectory(const SetDirectoryInfo * info);

		virtual ssize_t SetFindList(const SetFindListInfo * info);
	};

}

#endif
