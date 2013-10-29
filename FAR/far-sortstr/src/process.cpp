/**
 sortstr: Sort strings in editor
 FAR3 plugin

 © 2013 Andrew Grechkin

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/

#include <process.hpp>
#include <lang.hpp>
#include <guid.hpp>
#include <globalinfo.hpp>

#include <libfar3/plugin.hpp>
#include <libfar3/editor.hpp>
#include <libfar3/dialog_builder.hpp>
#include <libbase/std.hpp>
#include <libbase/string.hpp>
#include <liblog/logger.hpp>

#include <functional>
#include <cmath>
#include <vector>
#include <tuple>
#include <cstdlib>

const PCWSTR EDITOR_EOL = nullptr;
const ssize_t NOTHING_SELECTED = -2;

bool is_whitespace(WCHAR ch)
{
	return Cstr::find(get_global_info()->edValue_Whitespaces, ch);
}

INT_PTR editor_set_string(ssize_t y, const ustring & str, PCWSTR eol)
{
	return Far::Editor::set_string(y, str.c_str(), str.size(), eol);
}

struct StringInfo {
	ustring str;
	ssize_t row;
	ssize_t start;
	ssize_t count;

	StringInfo(const EditorGetString & egs, const FarGlobalInfo * fgi) :
		str(egs.StringText, egs.StringLength),
		row(egs.StringNumber),
		start(0),
		count(egs.StringLength)
	{
//		LogNoise(L"str[%Id]: egs.SelStart: %Id, egs.SelEnd: %Id, egs.StringLength: %Id '%s'\n", strNum, egs.SelStart, egs.SelEnd, egs.StringLength, tmp.c_str());
		if (fgi->get_block_type() == BTYPE_COLUMN) {
			count = NOTHING_SELECTED;
			if (egs.SelStart < egs.StringLength) {
				count = std::min(egs.SelEnd, egs.StringLength) - std::min(egs.SelStart, egs.StringLength);
			}
		}
		LogNoise(L"start: %Id, count: %Id '%s'\n", start, count, str.c_str());
	}
};

struct SortInfo {
	ustring     substr;
	size_t      line;
	long double num;

	SortInfo(const StringInfo & si, const FarGlobalInfo * fgi) :
		substr(get_substr(si, fgi->get_block_type(), fgi->cbValue_AsEmpty)),
		line(si.row - fgi->get_first_line()),
		num(0)
	{
		if (fgi->cbValue_Comparation == Comparation::NUMERIC) {
			num = FindNum(substr.c_str());
		}
//		LogNoise(L"sortdata.back().second.line: %Id, sortdata.back().second.num: %f, sortdata.back().first: '%s'\n", sortdata.back().line, (double )sortdata.back().num, sortdata.back().substr.c_str());
	}

private:
	static ustring get_substr(const StringInfo & si, EDITOR_BLOCK_TYPES blockType, bool asEmpty);

	static long double FindNum(PCWSTR str);
};

ustring SortInfo::get_substr(const StringInfo & si, EDITOR_BLOCK_TYPES blockType, bool asEmpty)
{
	if (blockType == BTYPE_COLUMN) {
		if (si.count != NOTHING_SELECTED) {
			return ustring(si.str.c_str() + si.start, si.count);
		} else if (asEmpty) {
			return ustring();
		}
	}
	return si.str;
}

long double SortInfo::FindNum(PCWSTR str)
{
	long double ret = HUGE_VAL;
	PCWSTR num = (PCWSTR)(str + ::wcscspn(str, L"0123456789"));

	if (*num) {
		WCHAR buf[132];
		buf[Base::lengthof(buf) - 1] = 0;
		Cstr::copy(buf, num, Base::lengthof(buf) - 1);
		for (PWSTR k = buf; *k; ++k) {
			if (*k == L',') {
				*k = L'.';
				break;
			}
		}
		PWSTR k = buf;
		for (PWSTR l = buf; *k && *l; ++l) {
			if (!is_whitespace(*l)) {
				*k++ = *l;
			}
		}
		*k = 0;
		ret = wcstold(buf, nullptr);
		if (num > str && num[-1] == L'-') {
			ret = -ret;
		}
	}
	return ret;
}


typedef std::vector<StringInfo> data_vector;
typedef std::vector<SortInfo> sort_vector;

bool PairEqCI(const SortInfo & left, const SortInfo & right)
{
	return Cstr::compare_ci(left.substr.c_str(), right.substr.c_str()) == 0;
}

bool PairEqCS(const SortInfo & left, const SortInfo & right)
{
	return Cstr::compare_cs(left.substr.c_str(), right.substr.c_str()) == 0;
}

bool PairEqCScode(const SortInfo & left, const SortInfo & right)
{
	return Cstr::compare(left.substr.c_str(), right.substr.c_str()) == 0;
}

bool PairEqNum(const SortInfo & left, const SortInfo & right)
{
	return left.num == right.num;
}

bool PairLessLine(const SortInfo & left, const SortInfo & right)
{
	return left.line < right.line;
}

bool PairEqLength(const SortInfo & left, const SortInfo & right)
{
	return left.substr.length() == right.substr.length();
}

bool PairLessCI(const SortInfo & left, const SortInfo & right)
{
	int ret = Cstr::compare_ci(left.substr.c_str(), right.substr.c_str());
	if (ret < 0)
		return true;
	else if (ret == 0)
		return left.line < right.line;
	return false;
}

bool LoggedPairLessCS(const SortInfo & left, const SortInfo & right)
{
	int ret = Cstr::compare_cs(left.substr.c_str(), right.substr.c_str());
	if (ret < 0)
		return true;
	else if (ret == 0)
		return left.line < right.line;
	return false;
}

bool PairLessCS(const SortInfo & left, const SortInfo & right)
{
	bool ret = LoggedPairLessCS(left, right);
//	LogNoise(L"'%s' < '%s' -> %d\n", left.substr.c_str(), right.substr.c_str(), ret);
	return ret;
}

bool PairLessCScode(const SortInfo & left, const SortInfo & right)
{
	int ret = Cstr::compare(left.substr.c_str(), right.substr.c_str());
	if (ret < 0)
		return true;
	else if (ret == 0)
		return left.line < right.line;
	return false;
}

bool PairLessNum(const SortInfo & left, const SortInfo & right)
{
	if (left.num < right.num)
		return true;
	else if (left.num == right.num)
		return left.line < right.line;
	return false;
}

bool PairLessLength(const SortInfo & left, const SortInfo & right)
{
	if (left.substr.length() < right.substr.length())
		return true;
	else if (left.substr.length() == right.substr.length())
		return left.line < right.line;
	return false;
}

template<typename Type>
void InsertFromVector(const data_vector & data, Type first, Type last)
{
	auto fgi = get_global_info();

	size_t i = fgi->get_first_line(), j = 0;
	for (; first != last; ++i, ++j) {
		if (data[j].count == NOTHING_SELECTED && !fgi->cbValue_AsEmpty) {
			continue;
		}
		if (j == first->line) {
			++first;
			continue;
		}
		switch (fgi->cbValue_Operation) {
			case Operation::SORT:
				if (fgi->get_block_type() == BTYPE_STREAM || !fgi->cbValue_Selected) {
					editor_set_string(i, data[first->line].str, EDITOR_EOL);
				} else {
					if (data[j].str.size() <= (size_t)data[j].start) {
						ustring tmp(data[j].start, Base::SPACE_C);
						tmp.replace(0, data[j].str.size(), data[j].str);
						tmp += first->substr;
						editor_set_string(i, tmp, EDITOR_EOL);
					} else {
						ustring tmp(data[j].str);
						tmp.replace(data[j].start, data[j].count, first->substr);
						editor_set_string(i, tmp, EDITOR_EOL);
					}
				}
				++first;
				break;
			case Operation::REMOVE_DUP:
				editor_set_string(i, data[first->line].str, EDITOR_EOL);
				++first;
				break;
			case Operation::SPARSE_DUP: {
				if (!data[j].str.empty()) {
					editor_set_string(i, Base::EMPTY_STR, EDITOR_EOL);
				}
				break;
			}
		}
	}
	switch (fgi->cbValue_Operation) {
		case Operation::SORT:
			break;
		case Operation::REMOVE_DUP:
			for (; j < data.size(); ++i, ++j)
				editor_set_string(i, Base::EMPTY_STR, EDITOR_EOL);
			break;
		case Operation::SPARSE_DUP:
			for (; j < data.size(); ++i, ++j)
				if (!data[j].str.empty())
					editor_set_string(i, Base::EMPTY_STR, EDITOR_EOL);
			break;
	}
}

bool Execute()
{
	auto fgi = get_global_info();

	LogNoise(L"get_first_line(): %Id, get_total_lines(): %Id\n", fgi->get_first_line(), fgi->get_total_lines());
	if (!fgi->get_total_lines())
		return false;

	LogNoise(L"get_block_type(): %Id\n", fgi->get_block_type());
	data_vector data;
	sort_vector sortdata;
//	data.reserve(fgi->get_total_lines() - fgi->get_first_line());
//	sortdata.reserve(data.capacity());

	EditorGetString egs;
	for (size_t editorRow = fgi->get_first_line(); editorRow < fgi->get_total_lines(); ++editorRow) {
		Far::Editor::get_string(editorRow, egs);

		if (editorRow == (fgi->get_total_lines() - 1) && Cstr::is_empty(egs.StringText))
			break;
		if (fgi->get_block_type() != BTYPE_NONE && (egs.SelStart == -1 || egs.SelStart == egs.SelEnd))
			break;

		data.emplace_back(egs, fgi);
		sortdata.emplace_back(data.back(), fgi);
	}

	LogTrace();
	std::pointer_to_binary_function<const SortInfo &, const SortInfo &, bool> pfLe;
	std::pointer_to_binary_function<const SortInfo &, const SortInfo &, bool> pfEq;
	switch (fgi->cbValue_Comparation) {
		case Comparation::LEX_CI:
			pfLe = std::ptr_fun(PairLessCI);
			pfEq = std::ptr_fun(PairEqCI);
			break;
		case Comparation::LEX_CS:
			pfLe = std::ptr_fun(PairLessCS);
			pfEq = std::ptr_fun(PairEqCS);
			break;
		case Comparation::LEX_CODE:
			pfLe = std::ptr_fun(PairLessCScode);
			pfEq = std::ptr_fun(PairEqCScode);
			break;
		case Comparation::NUMERIC:
			pfLe = std::ptr_fun(PairLessNum);
			pfEq = std::ptr_fun(PairEqNum);
			break;
		case Comparation::LENGTH:
			pfLe = std::ptr_fun(PairLessLength);
			pfEq = std::ptr_fun(PairEqLength);
			break;
	}

	std::sort(sortdata.begin(), sortdata.end(), pfLe);

	if (fgi->cbValue_Operation != Operation::SORT) {
		sort_vector::iterator it = std::unique(sortdata.begin(), sortdata.end(), pfEq);
		sortdata.erase(it, sortdata.end());
		std::sort(sortdata.begin(), sortdata.end(), std::ptr_fun(PairLessLine));
	}

//	for (sort_vector::iterator it = sortdata.begin(); it != sortdata.end(); ++it) {
//		LogDebug(L"line: %Iu, str: '%s'\n", it->second.line, it->first.c_str());
//	}

	Far::Editor::start_undo();
	if (fgi->cbValue_Invert && fgi->cbValue_Operation == Operation::SORT) {
		InsertFromVector(data, sortdata.rbegin(), sortdata.rend());
	} else {
		InsertFromVector(data, sortdata.begin(), sortdata.end());
	}
	Far::Editor::stop_undo();

	Far::Editor::redraw();

	return true;
}

void process()
{
	auto fgi = get_global_info();

	fgi->load_settings();
	fgi->load_editor_info();

	FarListItem cbOperation[] = {
		{0, Far::get_msg(lbSort), {0}},
		{0, Far::get_msg(lbRemoveDup), {0}},
		{0, Far::get_msg(lbSparseDup), {0}},
	};

	FarListItem cbComparation[] = {
		{0, Far::get_msg(lbCompLexCI), {0}},
		{0, Far::get_msg(lbCompLexCS), {0}},
		{0, Far::get_msg(lbCompLexCode), {0}},
		{0, Far::get_msg(lbCompNumeric), {0}},
		{0, Far::get_msg(lbCompLength), {0}},
	};

	using namespace Far;
	auto Builder = create_dialog_builder(DialogGuid, get_msg(DlgTitle));
	LogTrace();
	Builder->add_item(create_label(txOperation));
	Builder->add_item(create_combobox(reinterpret_cast<ssize_t*>(&fgi->cbValue_Operation), cbOperation, Base::lengthof(cbOperation), DIF_DROPDOWNLIST | DIF_LISTNOAMPERSAND));
	Builder->add_item(create_label(txComparation));
	Builder->add_item(create_combobox(reinterpret_cast<ssize_t*>(&fgi->cbValue_Comparation), cbComparation, Base::lengthof(cbComparation), DIF_DROPDOWNLIST | DIF_LISTNOAMPERSAND));
	Builder->add_item(create_separator());
	Builder->add_item(create_checkbox(&fgi->cbValue_Invert, cbInvert));
	Builder->add_item(create_label(txWhitespace));
	Builder->add_item_after(create_edit(fgi->edValue_Whitespaces, 10));
	Builder->add_item(create_separator());
	Builder->add_item(create_checkbox(&fgi->cbValue_Selected, cbSelected, (fgi->get_block_type() != BTYPE_COLUMN) ? DIF_DISABLE : 0));
	Builder->add_item(create_checkbox(&fgi->cbValue_AsEmpty, cbAsEmpty, (fgi->get_block_type() != BTYPE_COLUMN) ? DIF_DISABLE : 0));
	Builder->add_item(create_separator());
	Builder->add_OKCancel(get_msg(txtBtnOk), get_msg(txtBtnCancel));
	if (Builder->show()) {
		fgi->save_settings();
		Execute();
	}
}
