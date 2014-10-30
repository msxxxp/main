
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

#ifndef _FAR3_DIALOG_BUIDER_HPP_
#define _FAR3_DIALOG_BUIDER_HPP_

#include <far3/dialog.hpp>

namespace far3 {
	namespace dialog {

		class Builder_i {
		public:
			virtual ~Builder_i() = default;

			virtual Item& add_item(const Item& item) = 0;

			virtual Item& add_item_before(const Item& item) = 0;

			virtual Item& add_item_after(const Item& item) = 0;

			virtual void add_empty_line() = 0;

			virtual void add_OKCancel(PCWSTR OKLabel, PCWSTR CancelLabel, PCWSTR ExtraLabel = nullptr) = 0;

//			virtual void add_radiobuttons(ssize_t * Value, ssize_t OptionCount, const AddRadioButton_t list[], bool FocusOnSelected = false) = 0;

			virtual void start_column() = 0;

			virtual void break_column() = 0;

			virtual void end_column() = 0;

			virtual void start_singlebox(ssize_t Width, PCWSTR Label, bool LeftAlign = false) = 0;

			virtual void end_singlebox() = 0;

			virtual int show() = 0;
		};

		typedef simstd::unique_ptr<Builder_i> Builder;

		Builder create_builder(const GUID& aId, PCWSTR TitleLabel, PCWSTR aHelpTopic = nullptr, FARWINDOWPROC aDlgProc = nullptr, void * aUserParam = nullptr);

	}
}

#endif
