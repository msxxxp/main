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
#include <cstdlib>

PCWSTR EDITOR_EOL = nullptr;

struct SelInfo {
	ssize_t start;
	ssize_t count;

	SelInfo(ssize_t s = -1, ssize_t c = -1) :
		start(s),
		count(c)
	{
	}
};

struct SortInfo {
	size_t      line;
	long double num;

	SortInfo(size_t l) :
		line(l),
		num(0)
	{
	}
};

typedef std::pair<ustring, SelInfo> datapair;
typedef std::pair<ustring, SortInfo> sortpair;
typedef std::vector<datapair> data_vector;
typedef std::vector<sortpair> sort_vector;

bool PairEqCI(const sortpair & lhs, const sortpair & rhs)
{
	return Cstr::compare_ci(lhs.first.c_str(), rhs.first.c_str()) == 0;
}

bool PairEqCS(const sortpair & lhs, const sortpair & rhs)
{
	return Cstr::compare_cs(lhs.first.c_str(), rhs.first.c_str()) == 0;
}

bool PairEqCScode(const sortpair & lhs, const sortpair & rhs)
{
	return Cstr::compare(lhs.first.c_str(), rhs.first.c_str()) == 0;
}

bool PairEqNum(const sortpair & lhs, const sortpair & rhs)
{
	return lhs.second.num == rhs.second.num;
}

bool PairLessLine(const sortpair & lhs, const sortpair & rhs)
{
	return lhs.second.line < rhs.second.line;
}

bool PairEqLength(const sortpair & lhs, const sortpair & rhs)
{
	return lhs.first.length() == rhs.first.length();
}

bool PairLessCI(const sortpair & lhs, const sortpair & rhs)
{
	int ret = Cstr::compare_ci(lhs.first.c_str(), rhs.first.c_str());
	if (ret < 0)
		return true;
	else if (ret == 0)
		return lhs.second.line < rhs.second.line;
	return false;
}

bool LoggedPairLessCS(const sortpair & lhs, const sortpair & rhs)
{
	int ret = Cstr::compare_cs(lhs.first.c_str(), rhs.first.c_str());
	if (ret < 0)
		return true;
	else if (ret == 0)
		return lhs.second.line < rhs.second.line;
	return false;
}

bool PairLessCS(const sortpair & left, const sortpair & right)
{
	bool ret = LoggedPairLessCS(left, right);
	LogNoise(L"'%s' < '%s' -> %d\n", left.first.c_str(), right.first.c_str(), ret);
	return ret;
}

bool PairLessCScode(const sortpair & lhs, const sortpair & rhs)
{
	int ret = Cstr::compare(lhs.first.c_str(), rhs.first.c_str());
	if (ret < 0)
		return true;
	else if (ret == 0)
		return lhs.second.line < rhs.second.line;
	return false;
}

bool PairLessNum(const sortpair & lhs, const sortpair & rhs)
{
	if (lhs.second.num < rhs.second.num)
		return true;
	else if (lhs.second.num == rhs.second.num)
		return lhs.second.line < rhs.second.line;
	return false;
}

bool PairLessLength(const sortpair & lhs, const sortpair & rhs)
{
	return lhs.first.length() < rhs.first.length();
}

INT_PTR editor_set_string(ssize_t y, ustring const& str, PCWSTR eol)
{
	return Far::Editor::set_string(y, str.c_str(), str.size(), eol);
}

bool is_whitespace(WCHAR ch)
{
	return Cstr::find(get_global_info()->edValue_Whitespaces, ch);
}

long double FindNum(PCWSTR str)
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

template<typename Type>
void InsertFromVector(const data_vector & data, Type first, Type last)
{
	size_t i = get_global_info()->get_first_line(), j = 0;
	for (; first != last; ++i, ++j) {
		if (data[j].second.count == -2 && !get_global_info()->cbValue_AsEmpty) {
			continue;
		}
		if (j == first->second.line) {
			++first;
			continue;
		}
		switch (get_global_info()->cbValue_Operation) {
			case Operation::SORT:
				if (get_global_info()->get_block_type() == BTYPE_STREAM || !get_global_info()->cbValue_Selected) {
					editor_set_string(i, data[first->second.line].first, EDITOR_EOL);
				} else {
					if (data[j].first.size() <= (size_t)data[j].second.start) {
						ustring tmp(data[j].second.start, Base::SPACE_C);
						tmp.replace(0, data[j].first.size(), data[j].first);
						tmp += first->first;
						editor_set_string(i, tmp, EDITOR_EOL);
					} else {
						ustring tmp(data[j].first);
						tmp.replace(data[j].second.start, data[j].second.count, first->first);
						editor_set_string(i, tmp, EDITOR_EOL);
					}
				}
				++first;
				break;
			case Operation::REMOVE_DUP:
				editor_set_string(i, data[first->second.line].first, EDITOR_EOL);
				++first;
				break;
			case Operation::SPARSE_DUP: {
				if (!data[j].first.empty()) {
					editor_set_string(i, Base::EMPTY_STR, EDITOR_EOL);
				}
				break;
			}
		}
	}
	switch (get_global_info()->cbValue_Operation) {
		case Operation::SORT:
			break;
		case Operation::REMOVE_DUP:
			for (; j < data.size(); ++i, ++j)
				editor_set_string(i, Base::EMPTY_STR, EDITOR_EOL);
			break;
		case Operation::SPARSE_DUP:
			for (; j < data.size(); ++i, ++j)
				if (!data[j].first.empty())
					editor_set_string(i, Base::EMPTY_STR, EDITOR_EOL);
			break;
	}
}

bool Execute()
{
	LogNoise(L"get_first_line(): %Id, get_total_lines(): %Id\n", get_global_info()->get_first_line(), get_global_info()->get_total_lines());
	if (!get_global_info()->get_total_lines())
		return false;

	LogNoise(L"get_block_type(): %Id\n", get_global_info()->get_block_type());
	data_vector data;
	sort_vector sortdata;
//	data.reserve(get_global_info()->get_total_lines() - get_global_info()->get_first_line());
//	sortdata.reserve(data.capacity());

	EditorGetString egs;
	for (size_t strNum = get_global_info()->get_first_line(); strNum < get_global_info()->get_total_lines(); ++strNum) {
		Far::Editor::get_string(strNum, egs);

		if (strNum == (get_global_info()->get_total_lines() - 1) && Cstr::is_empty(egs.StringText))
			break;
		if (get_global_info()->get_block_type() != BTYPE_NONE && (egs.SelStart == -1 || egs.SelStart == egs.SelEnd))
			break;

		ustring tmp(egs.StringText, egs.StringLength);
		LogDebug(L"str[%Id]: egs.SelStart: %Id, egs.SelEnd: %Id, egs.StringLength: %Id '%s'\n", strNum, egs.SelStart, egs.SelEnd, egs.StringLength, tmp.c_str());

		ssize_t SelLen = -2;
		switch (get_global_info()->get_block_type()) {
			case BTYPE_COLUMN: {
				if (egs.SelStart < egs.StringLength) {
					SelLen = std::min(egs.SelEnd, egs.StringLength) - std::min(egs.SelStart, egs.StringLength);
				}
				LogDebug(L"SelLen: %Id\n", SelLen);
				if (SelLen != -2) {
					sortdata.emplace_back(ustring(egs.StringText + egs.SelStart, SelLen), strNum - get_global_info()->get_first_line());
					if (get_global_info()->cbValue_Comparation == Comparation::NUMERIC) {
						sortdata.back().second.num = FindNum(sortdata.back().first.c_str());
					}
					LogNoise(L"sortdata.back().second.line: %Id, sortdata.back().second.num: %f, sortdata.back().first: '%s'\n", sortdata.back().second.line, (double )sortdata.back().second.num, sortdata.back().first.c_str());
				} else if (get_global_info()->cbValue_AsEmpty) {
					sortdata.emplace_back(ustring(), strNum - get_global_info()->get_first_line());
					LogNoise(L"sortdata.back().second.line: %Id, sortdata.back().second.num: %f, sortdata.back().first: '%s'\n", sortdata.back().second.line, (double )sortdata.back().second.num, sortdata.back().first.c_str());
				}
				data.emplace_back(tmp, SelInfo(egs.SelStart, SelLen));
				LogNoise(L"data.back().second.start: %Id, data.back().second.count: %Id, data.back().first: '%s'\n", data.back().second.start, data.back().second.count, data.back().first.c_str());
				break;
			}
			case BTYPE_STREAM:
			default: {
				data.emplace_back(tmp, SelInfo(0, egs.StringLength));
				sortdata.emplace_back(tmp, strNum - get_global_info()->get_first_line());
				if (get_global_info()->cbValue_Comparation == Comparation::NUMERIC) {
					sortdata.back().second.num = FindNum(sortdata.back().first.c_str());
				}
				LogNoise(L"sortdata.back().second.line: %Id, sortdata.back().second.num: %f, sortdata.back().first: '%s'\n", sortdata.back().second.line, (double )sortdata.back().second.num, sortdata.back().first.c_str());
				LogNoise(L"data.back().second.start: %Id, data.back().second.count: %Id, data.back().first: '%s'\n", data.back().second.start, data.back().second.count, data.back().first.c_str());
				break;
			}
		}
	}

	LogTrace();
	std::pointer_to_binary_function<const sortpair &, const sortpair &, bool> pfLe;
	std::pointer_to_binary_function<const sortpair &, const sortpair &, bool> pfEq;
	switch (get_global_info()->cbValue_Comparation) {
		case Comparation::LEX_CI:
			pfLe = ptr_fun(PairLessCI);
			pfEq = ptr_fun(PairEqCI);
			break;
		case Comparation::LEX_CS:
			pfLe = ptr_fun(PairLessCS);
			pfEq = ptr_fun(PairEqCS);
			break;
		case Comparation::LEX_CODE:
			pfLe = ptr_fun(PairLessCScode);
			pfEq = ptr_fun(PairEqCScode);
			break;
		case Comparation::NUMERIC:
			pfLe = ptr_fun(PairLessNum);
			pfEq = ptr_fun(PairEqNum);
			break;
		case Comparation::LENGTH:
			pfLe = ptr_fun(PairLessLength);
			pfEq = ptr_fun(PairEqLength);
			break;
	}

	std::stable_sort(sortdata.begin(), sortdata.end(), pfLe);

	if (get_global_info()->cbValue_Operation != Operation::SORT) {
		sort_vector::iterator it = std::unique(sortdata.begin(), sortdata.end(), pfEq);
		sortdata.erase(it, sortdata.end());
		std::stable_sort(sortdata.begin(), sortdata.end(), ptr_fun(PairLessLine));
	}

//	for (sort_vector::iterator it = sortdata.begin(); it != sortdata.end(); ++it) {
//		LogDebug(L"line: %Iu, str: '%s'\n", it->second.line, it->first.c_str());
//	}

	Far::Editor::start_undo();
	if (get_global_info()->cbValue_Invert && get_global_info()->cbValue_Operation == Operation::SORT) {
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
	get_global_info()->load_settings();
	get_global_info()->load_editor_info();

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
	Builder->add_item(create_combobox(reinterpret_cast<ssize_t*>(&get_global_info()->cbValue_Operation), cbOperation, Base::lengthof(cbOperation), DIF_DROPDOWNLIST | DIF_LISTNOAMPERSAND));
	Builder->add_item(create_label(txComparation));
	Builder->add_item(create_combobox(reinterpret_cast<ssize_t*>(&get_global_info()->cbValue_Comparation), cbComparation, Base::lengthof(cbComparation), DIF_DROPDOWNLIST | DIF_LISTNOAMPERSAND));
	Builder->add_item(create_separator());
	Builder->add_item(create_checkbox(&get_global_info()->cbValue_Invert, cbInvert));
	Builder->add_item(create_label(txWhitespace));
	Builder->add_item_after(create_edit(get_global_info()->edValue_Whitespaces, 10));
	Builder->add_item(create_separator());
	Builder->add_item(create_checkbox(&get_global_info()->cbValue_Selected, cbSelected, (get_global_info()->get_block_type() != BTYPE_COLUMN) ? DIF_DISABLE : 0));
	Builder->add_item(create_checkbox(&get_global_info()->cbValue_AsEmpty, cbAsEmpty, (get_global_info()->get_block_type() != BTYPE_COLUMN) ? DIF_DISABLE : 0));
	Builder->add_item(create_separator());
	Builder->add_OKCancel(get_msg(txtBtnOk), get_msg(txtBtnCancel));
	if (Builder->show()) {
		get_global_info()->save_settings();
		Execute();
	}
}
