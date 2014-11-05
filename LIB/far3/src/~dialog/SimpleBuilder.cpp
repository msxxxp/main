
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
#include <basis/simstd/vector>

//#include "DlgBuilder_pvt.hpp"

namespace far3 {
	namespace dialog {

	bool CompareX2_less(const Item & a, const Item & b)
	{
		return a.X2 < b.X2;
	}

	bool CompareWidth_less(const Item & a, const Item & b)
	{
		LogNoise(L"(%d, %s) cmp (%d, %s)\n", a.Type, a.Data, b.Type, b.Data);
		return (a.X1 + a.get_width()) < (b.X1 + b.get_width());
	}

	///=============================================================================================
	class SimpleBuilder: public Builder_i {
	public:
		~SimpleBuilder();

		SimpleBuilder(size_t count, const GUID& guid, PCWSTR Label, PCWSTR aHelpTopic, FARWINDOWPROC aDlgProc, void* aUserParam);

		bool is_valid() const;

		Item& add_item(Item&& item) override;

		Item& add_item_before(Item&& item) override;

		Item& add_item_after(Item&& item) override;

//		void add_radiobuttons(ssize_t * Value, ssize_t OptionCount, const AddRadioButton_t list[], bool FocusOnSelected) override;

		void add_empty_line() override;

		void add_OKCancel(PCWSTR OKLabel, PCWSTR CancelLabel, PCWSTR ExtraLabel) override;

		void start_column() override;

		void break_column() override;

		void end_column() override;

		void start_singlebox(ssize_t Width, PCWSTR Label, bool LeftAlign) override;

		void end_singlebox() override;

		int show() override;

	protected:
		Item& add_dialog_item(ssize_t min_width, FARDIALOGITEMTYPES Type, PCWSTR Text, FARDIALOGITEMFLAGS flags = DIF_NONE);

		Item& add_dialog_item(Item&& item);

		ssize_t GetMaxItemX2() const;

		void save();

		void set_next_y(Item& item);

		int show_dialog_();

		void UpdateBorderSize();

	protected:
		GUID m_guid;
		FARWINDOWPROC DlgProc;
		PCWSTR HelpTopic;
		void * UserParam;

		HANDLE DialogHandle;

		simstd::vector<Item> DialogItems;
		size_t DialogItemsAllocated;

		ssize_t NextY;
		ssize_t Indent;

		ssize_t OKButtonId;
		ssize_t SingleBoxIndex;

	private:
		static const ssize_t DEFAULT_BORDER_INDENT_X = 3;
		static const ssize_t DEFAULT_BORDER_INDENT_Y = 1;
		static const ssize_t ZERO_X = 4;
		static const ssize_t ZERO_Y = 2;
		static const ssize_t DEFAULT_PADDING = 1;
	};

	SimpleBuilder::~SimpleBuilder()
	{
		psi().DialogFree(DialogHandle);

//		simstd::for_each(DialogItems.begin(), DialogItems.end(), std::bind(&Item::destroy, std::placeholders::_1));
		LogTrace();
	}

	SimpleBuilder::SimpleBuilder(size_t count, const GUID & guid, PCWSTR label, PCWSTR aHelpTopic, FARWINDOWPROC aDlgProc, void * aUserParam) :
		m_guid(guid),
		DlgProc(aDlgProc),
		HelpTopic(aHelpTopic),
		UserParam(aUserParam),
		DialogHandle(nullptr),
		DialogItemsAllocated(0),
		NextY(ZERO_Y),
		Indent(0),
		OKButtonId(-1),
		SingleBoxIndex(0)
	{
		LogNoise(L"'%s'\n", label);

		DialogItems.reserve(count);

		// create border
		Item& item(add_dialog_item(22, DI_DOUBLEBOX, label));
		item.set_dimensions(DEFAULT_BORDER_INDENT_X, DEFAULT_BORDER_INDENT_Y, item.get_width() + 4, 4);
		LogNoise(L"NextY: %Id, size: %Iu\n", NextY, DialogItems.size());
	}

	bool SimpleBuilder::is_valid() const
	{
//		LogErrorIf(!DialogHandle, L"handle is invalid\n");
//		return DialogHandle;
		return true;
	}

	Item& SimpleBuilder::add_item(Item&& it)
	{
		LogTrace();
		Item& ret(add_dialog_item(simstd::move(it)));
		set_next_y(ret);

		return ret;
	}

	Item& SimpleBuilder::add_item_before(Item&& it)
	{
		Item& item(add_dialog_item(simstd::move(it)));
		Item& RelativeTo(DialogItems[DialogItems.size() - 2]);
		item.Y1 = item.Y2 = RelativeTo.Y1;
		item.X1 = RelativeTo.X1;
		item.X2 = item.X1 + item.get_width() - 1;

		ssize_t RelativeToWidth = RelativeTo.X2 - RelativeTo.X1;
		RelativeTo.X1 = item.X2 + DEFAULT_PADDING + 1;
		RelativeTo.X2 = RelativeTo.X1 + RelativeToWidth;

		return item;
	}

	Item& SimpleBuilder::add_item_after(Item&& it)
	{
		LogTrace();
		Item& item(add_dialog_item(simstd::move(it)));
		Item& RelativeTo(DialogItems[DialogItems.size() - 2]);
		item.Y1 = item.Y2 = RelativeTo.Y1;
		ssize_t ItemWidth = item.X2 - item.X1;
		item.X1 = RelativeTo.X1 + RelativeTo.get_width() - 1 + DEFAULT_PADDING + 1;
		item.X2 = item.X1 + ItemWidth;
		return item;
	}

//	void SimpleBuilder::add_radiobuttons(ssize_t * Value, ssize_t OptionCount, const AddRadioButton_t list[], bool FocusOnSelected)
//	{
//		ssize_t rg_index = 0;
//		for (ssize_t i = 0; i < OptionCount; ++i) {
//			Item * item = add_dialog_item(new Item(new PluginRadioButtonBinding(DialogHandle, DialogItems.size() - 1, Value, rg_index++), DI_RADIOBUTTON, message::get(list[i].id), list[i].flags));
//			set_next_y(item);
//			item->X2 = item->X1 + item->get_width();
//			if (!i)
//				item->Flags |= DIF_GROUP;
//			if (*Value == i) {
//				item->Selected = TRUE;
//				if (FocusOnSelected)
//					item->Flags |= DIF_FOCUS;
//			}
//		}
//	}

	void SimpleBuilder::add_empty_line()
	{
		LogTrace();
		NextY++;
	}

	void SimpleBuilder::add_OKCancel(PCWSTR OKLabel, PCWSTR CancelLabel, PCWSTR ExtraLabel)
	{
		LogTrace();
		Item& OKButton(add_dialog_item(-1, DI_BUTTON, OKLabel, DIF_CENTERGROUP | DIF_DEFAULTBUTTON));
		OKButton.Y1 = OKButton.Y2 = NextY++;
		OKButtonId = DialogItems.size() - 1;

		if (!cstr::is_empty(CancelLabel)) {
			Item& CancelButton(add_dialog_item(-1, DI_BUTTON, CancelLabel, DIF_CENTERGROUP));
			CancelButton.Y1 = CancelButton.Y2 = OKButton.Y1;
		}

		if (!cstr::is_empty(ExtraLabel)) {
			Item& ExtraButton(add_dialog_item(-1, DI_BUTTON, ExtraLabel, DIF_CENTERGROUP));
			ExtraButton.Y1 = ExtraButton.Y2 = OKButton.Y1;
		}
	}

	void SimpleBuilder::start_column()
	{
//		ColumnStartIndex = DialogItemsCount;
//		ColumnStartY = NextY;
	}

	void SimpleBuilder::break_column()
	{
//		ColumnBreakIndex = DialogItemsCount;
//		NextY = ColumnStartY;
	}

	void SimpleBuilder::end_column()
	{
//		ssize_t colWidth = 0;
//		for (int i = ColumnStartIndex; i < ColumnBreakIndex; ++i) {
//			colWidth = simstd::max(colWidth, ItemWidth(&DialogItems[i]));
//		}
//		for (size_t i = ColumnBreakIndex; i < DialogItemsCount; ++i) {
//			DialogItems[i].X1 += (1 + colWidth);
//			DialogItems[i].X2 += (1 + colWidth);
//		}
//
//		ColumnStartIndex = -1;
//		ColumnBreakIndex = -1;
	}

	void SimpleBuilder::start_singlebox(ssize_t Width, PCWSTR Label, bool LeftAlign)
	{
		Item& SingleBox(add_dialog_item(-1, DI_SINGLEBOX, Label));
		SingleBox.Flags = LeftAlign ? DIF_LEFTTEXT : DIF_NONE;
		SingleBox.X1 = ZERO_X + DEFAULT_PADDING + Indent;
		SingleBox.X2 = SingleBox.X1 + Width;
		SingleBox.Y1 = NextY++;
		Indent += 2;
		SingleBoxIndex = DialogItems.size() - 1;
	}

	void SimpleBuilder::end_singlebox()
	{
		if (SingleBoxIndex) {
			DialogItems[SingleBoxIndex].Y2 = NextY++;
			SingleBoxIndex = 0;
			Indent -= 2;
		}
	}

	int SimpleBuilder::show()
	{
		LogTrace();
		UpdateBorderSize();
		LogTrace();
		int Result = show_dialog_();
		if (Result == OKButtonId) {
			save();
		}
		if (Result >= OKButtonId) {
			Result -= OKButtonId;
		}
		return Result;
	}


	///---------------------------------------------------------------------------------------------
	Item& SimpleBuilder::add_dialog_item(ssize_t min_width, FARDIALOGITEMTYPES type, PCWSTR text, FARDIALOGITEMFLAGS flags)
	{
		LogTrace();
		LogFatalIf(DialogItems.size() == DialogItems.capacity(), L"dialog items container will be replaced\n");
		CRT_ASSERT(DialogItems.size() != DialogItems.capacity());
		DialogItems.emplace_back(min_width, type, text, flags);
		Item& ret = DialogItems.back();
		ret.set_dialog(DialogHandle);
		ret.set_index(DialogItems.size() - 1);

		LogNoise(L"DialogItems.size(): %Iu\n", DialogItems.size());
		return ret;
	}

	Item& SimpleBuilder::add_dialog_item(Item&& item)
	{
		CRT_ASSERT(DialogItems.size() != DialogItems.capacity());
		LogTrace();
		DialogItems.emplace_back(simstd::move(item));

		Item& ret = DialogItems.back();
		ret.set_dialog(DialogHandle);
		ret.set_index(DialogItems.size() - 1);

		return ret;
	}

	ssize_t SimpleBuilder::GetMaxItemX2() const
	{
		LogTrace();
		auto it = simstd::max_element(++DialogItems.begin(), DialogItems.end(), &CompareWidth_less);
		ssize_t ret = it->X1 + it->get_width() - 1 - ZERO_X;
		LogNoise(L"-> %Id\n", ret);
		return ret;
	}

	void SimpleBuilder::save()
	{
		LogTrace();
		for (Item& it : DialogItems) {
			it.save();
		}
	}

	void SimpleBuilder::set_next_y(Item& item)
	{
		LogTrace();
		item.set_dimensions(ZERO_X + DEFAULT_PADDING + Indent, NextY++, (item.X2) ? item.X2 : item.get_width());
		LogNoise(L"NextY: %Id\n", NextY);

//		Item->X1 = ZERO_X + DEFAULT_PADDING + Indent;
//		if (Item->X2)
//			Item->X2 += Item->X1;
//		Item->Y1 = NextY++;
//		Item->Y2 = Item->Y1;
//		Item->Y2 = Item->Y1 + Item->get_height() - 1;
//		NextY = Item->Y2 + 1;
	}

	int SimpleBuilder::show_dialog_()
	{
		LogTrace();
		ssize_t Width = DialogItems[0].X2 + (DEFAULT_BORDER_INDENT_X  + 1);
		ssize_t Height = DialogItems[0].Y2 + (DEFAULT_BORDER_INDENT_Y  + 1);
		DialogHandle = psi().DialogInit(get_plugin_guid(), &m_guid, -1, -1, Width, Height, HelpTopic, &DialogItems[0], DialogItems.size(), 0, 0, DlgProc, UserParam);
		return psi().DialogRun(DialogHandle);
	}

	void SimpleBuilder::UpdateBorderSize()
	{
		LogTrace();
		if (DialogItems.size() > 1) {
			Item & border = DialogItems[0];
			auto borderX2 = GetMaxItemX2() + DEFAULT_PADDING + 1;
			LogTrace();
			borderX2 -= (border.X2 - border.X1);
			LogTrace();
			if (borderX2 > 0)
				border.X2 += borderX2;
			LogTrace();
			border.Y2 = DialogItems.back().Y2 + 1;
		}
		LogTrace();
	}

	///=============================================================================================
	Builder create_builder(size_t count, const GUID & aId, PCWSTR TitleLabel, PCWSTR aHelpTopic, FARWINDOWPROC aDlgProc, void * aUserParam)
	{
		LogTrace();
		simstd::unique_ptr<SimpleBuilder> tmp(new SimpleBuilder(count, aId, TitleLabel, aHelpTopic, aDlgProc, aUserParam));
		return tmp->is_valid() ? Builder(simstd::move(tmp)) : Builder();
	}

}}
