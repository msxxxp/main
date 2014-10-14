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

#include <basis/sys/cstr.hpp>
#include <basis/sys/logger.hpp>

#include <basis/simstd/algorithm>
#include <basis/simstd/memory>
#include <memory>

#include "DlgBuilder_pvt.hpp"

namespace Far {

	bool CompareX2_less(const FarDialogItem_t & a, const FarDialogItem_t & b)
	{
		return a.X2 < b.X2;
	}

	bool CompareWidth_less(const FarDialogItem_t & a, const FarDialogItem_t & b)
	{
		LogNoise(L"(%d, %s) cmp (%d, %s)\n", a.Type, a.Data, b.Type, b.Data);
		return (a.X1 + a.get_width()) < (b.X1 + b.get_width());
	}

	///=============================================================================================
	SimpleDialogBuilder_impl::~SimpleDialogBuilder_impl()
	{
		psi().DialogFree(DialogHandle);

//		simstd::for_each(DialogItems.begin(), DialogItems.end(), std::bind(&FarDialogItem_t::destroy, std::placeholders::_1));
		LogTrace();
	}

	SimpleDialogBuilder_impl::SimpleDialogBuilder_impl(const GUID & guid, PCWSTR label, PCWSTR aHelpTopic, FARWINDOWPROC aDlgProc, void * aUserParam) :
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

		// create border
		auto item = add_dialog_item(DI_DOUBLEBOX, label);
		item->set_dimension(DEFAULT_BORDER_INDENT_X, DEFAULT_BORDER_INDENT_Y, item->get_width() + 4, 4);
		LogNoise(L"NextY: %Id, size: %Iu\n", NextY, DialogItems.size());
	}

	FarDialogItem_t * SimpleDialogBuilder_impl::add_item_(FarDialogItem_t * item)
	{
		LogTrace();
		FarDialogItem_t * ret = add_dialog_item(item);
		set_next_y(ret);

		return ret;
	}

	FarDialogItem_t * SimpleDialogBuilder_impl::add_item_before_(FarDialogItem_t * item)
	{
		FarDialogItem_t * Item = add_dialog_item(item);
		FarDialogItem_t * RelativeTo = &DialogItems[DialogItems.size() - 2];
		Item->Y1 = Item->Y2 = RelativeTo->Y1;
		Item->X1 = RelativeTo->X1;
		Item->X2 = Item->X1 + Item->get_width() - 1;

		ssize_t RelativeToWidth = RelativeTo->X2 - RelativeTo->X1;
		RelativeTo->X1 = Item->X2 + DEFAULT_PADDING + 1;
		RelativeTo->X2 = RelativeTo->X1 + RelativeToWidth;

		return Item;
	}

	FarDialogItem_t * SimpleDialogBuilder_impl::add_item_after_(FarDialogItem_t * item)
	{
		LogTrace();
		FarDialogItem_t * Item = add_dialog_item(item);
		FarDialogItem_t * RelativeTo = &DialogItems[DialogItems.size() - 2];
		Item->Y1 = Item->Y2 = RelativeTo->Y1;
		ssize_t ItemWidth = Item->X2 - Item->X1;
		Item->X1 = RelativeTo->X1 + RelativeTo->get_width() - 1 + DEFAULT_PADDING + 1;
		Item->X2 = Item->X1 + ItemWidth;
		return Item;
	}

	void SimpleDialogBuilder_impl::add_radiobuttons_(ssize_t * Value, ssize_t OptionCount, const AddRadioButton_t list[], bool FocusOnSelected)
	{
		ssize_t rg_index = 0;
		for (ssize_t i = 0; i < OptionCount; ++i) {
			FarDialogItem_t * Item = add_dialog_item(new FarDialogItem_t(new PluginRadioButtonBinding(DialogHandle, DialogItems.size() - 1, Value, rg_index++), DI_RADIOBUTTON, get_msg(list[i].id), list[i].flags));
			set_next_y(Item);
			Item->X2 = Item->X1 + Item->get_width();
			if (!i)
				Item->Flags |= DIF_GROUP;
			if (*Value == i) {
				Item->Selected = TRUE;
				if (FocusOnSelected)
					Item->Flags |= DIF_FOCUS;
			}
		}
	}

	void SimpleDialogBuilder_impl::add_empty_line_()
	{
		LogTrace();
		NextY++;
	}

	void SimpleDialogBuilder_impl::add_OKCancel_(PCWSTR OKLabel, PCWSTR CancelLabel, PCWSTR ExtraLabel)
	{
		LogTrace();
		FarDialogItem_t * OKButton = add_dialog_item(DI_BUTTON, OKLabel, DIF_CENTERGROUP | DIF_DEFAULTBUTTON);
		OKButton->Y1 = OKButton->Y2 = NextY++;
		OKButtonId = DialogItems.size() - 1;

		if (!cstr::is_empty(CancelLabel)) {
			FarDialogItem_t * CancelButton = add_dialog_item(DI_BUTTON, CancelLabel, DIF_CENTERGROUP);
			CancelButton->Y1 = CancelButton->Y2 = OKButton->Y1;
		}

		if (!cstr::is_empty(ExtraLabel)) {
			FarDialogItem_t * ExtraButton = add_dialog_item(DI_BUTTON, ExtraLabel, DIF_CENTERGROUP);
			ExtraButton->Y1 = ExtraButton->Y2 = OKButton->Y1;
		}
	}

	void SimpleDialogBuilder_impl::start_column_()
	{
//		ColumnStartIndex = DialogItemsCount;
//		ColumnStartY = NextY;
	}

	void SimpleDialogBuilder_impl::break_column_()
	{
//		ColumnBreakIndex = DialogItemsCount;
//		NextY = ColumnStartY;
	}

	void SimpleDialogBuilder_impl::end_column_()
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

	void SimpleDialogBuilder_impl::start_singlebox_(ssize_t Width, PCWSTR Label, bool LeftAlign)
	{
		FarDialogItem_t * SingleBox = add_dialog_item(DI_SINGLEBOX, Label);
		SingleBox->Flags = LeftAlign ? DIF_LEFTTEXT : DIF_NONE;
		SingleBox->X1 = ZERO_X + DEFAULT_PADDING + Indent;
		SingleBox->X2 = SingleBox->X1 + Width;
		SingleBox->Y1 = NextY++;
		Indent += 2;
		SingleBoxIndex = DialogItems.size() - 1;
	}

	void SimpleDialogBuilder_impl::end_singlebox_()
	{
		if (SingleBoxIndex) {
			DialogItems[SingleBoxIndex].Y2 = NextY++;
			SingleBoxIndex = 0;
			Indent -= 2;
		}
	}

	int SimpleDialogBuilder_impl::show_()
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
	FarDialogItem_t * SimpleDialogBuilder_impl::add_dialog_item(FARDIALOGITEMTYPES Type, PCWSTR Text, FARDIALOGITEMFLAGS flags)
	{
		LogTrace();
		DialogItems.emplace_back(Type, Text, flags);
		auto ret = &DialogItems.back();
		ret->set_dlg(&DialogHandle);
		ret->set_index(DialogItems.size() - 1);

		LogNoise(L"DialogItems.size(): %Iu\n", DialogItems.size());
		return ret;
	}

	FarDialogItem_t * SimpleDialogBuilder_impl::add_dialog_item(FarDialogItem_t * item)
	{
		LogTrace();
		DialogItems.emplace_back(simstd::move(*item));
		delete item;

		FarDialogItem_t * ret = &DialogItems.back();
		ret->set_dlg(&DialogHandle);
		ret->set_index(DialogItems.size() - 1);

		return ret;
	}

	ssize_t SimpleDialogBuilder_impl::GetMaxItemX2() const
	{
		LogTrace();
		auto it = simstd::max_element(++DialogItems.begin(), DialogItems.end(), &CompareWidth_less);
		ssize_t ret = it->X1 + it->get_width() - 1 - ZERO_X;
		LogNoise(L"-> %Id\n", ret);
		return ret;
	}

	void SimpleDialogBuilder_impl::save()
	{
		LogTrace();
		simstd::for_each(DialogItems.begin(), DialogItems.end(), std::bind(&FarDialogItem_t::save, std::placeholders::_1));
	}

	void SimpleDialogBuilder_impl::set_next_y(FarDialogItem_t * item)
	{
		LogTrace();
		item->set_dimension(ZERO_X + DEFAULT_PADDING + Indent, NextY++, (item->X2) ? item->X2 : item->get_width());
		LogNoise(L"NextY: %Id\n", NextY);

//		Item->X1 = ZERO_X + DEFAULT_PADDING + Indent;
//		if (Item->X2)
//			Item->X2 += Item->X1;
//		Item->Y1 = NextY++;
//		Item->Y2 = Item->Y1;
//		Item->Y2 = Item->Y1 + Item->get_height() - 1;
//		NextY = Item->Y2 + 1;
	}

	int SimpleDialogBuilder_impl::show_dialog_()
	{
		LogTrace();
		ssize_t Width = DialogItems[0].X2 + (DEFAULT_BORDER_INDENT_X  + 1);
		ssize_t Height = DialogItems[0].Y2 + (DEFAULT_BORDER_INDENT_Y  + 1);
		DialogHandle = psi().DialogInit(get_plugin_guid(), &m_guid, -1, -1, Width, Height, HelpTopic, &DialogItems[0], DialogItems.size(), 0, 0, DlgProc, UserParam);
		return psi().DialogRun(DialogHandle);
	}

	void SimpleDialogBuilder_impl::UpdateBorderSize()
	{
		LogTrace();
		if (DialogItems.size() > 1) {
			FarDialogItem_t & border = DialogItems[0];
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
	simstd::shared_ptr<DialogBuilder_i> create_dialog_builder(const GUID & aId, PCWSTR TitleLabel, PCWSTR aHelpTopic, FARWINDOWPROC aDlgProc, void * aUserParam)
	{
		LogTrace();
		return simstd::shared_ptr<DialogBuilder_i>(new SimpleDialogBuilder_impl(aId, TitleLabel, aHelpTopic, aDlgProc, aUserParam));
	}

}
