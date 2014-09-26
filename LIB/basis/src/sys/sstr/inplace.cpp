#include <basis/sys/cstr.hpp>
#include <basis/sys/sstr.hpp>

#include <basis/simstd/string>

namespace sstr {

	namespace inplace {

		astring & trim_left(astring & str, const astring & chrs)
		{
			astring::size_type pos = str.find_first_not_of(chrs);
			if (pos && pos != astring::npos) {
				str.erase(0, pos);
			}
			return str;
		}

		ustring & trim_left(ustring & str, const ustring & chrs)
		{
			ustring::size_type pos = str.find_first_not_of(chrs);
			if (pos && pos != ustring::npos) {
				str.erase(0, pos);
			}
			return str;
		}

		astring & trim_right(astring & str, const astring & chrs)
		{
			astring::size_type pos = str.find_last_not_of(chrs);
			if (pos != astring::npos && (++pos < str.size())) {
				str.erase(pos);
			}
			return str;
		}

		ustring & trim_right(ustring & str, const ustring & chrs)
		{
			ustring::size_type pos = str.find_last_not_of(chrs);
			if (pos != ustring::npos && (++pos < str.size())) {
				str.erase(pos);
			}
			return str;
		}

		astring & trim(astring & str, const astring & chrs)
		{
			trim_right(str, chrs);
			trim_left(str, chrs);
			return str;
		}

		ustring & trim(ustring & str, const ustring & chrs)
		{
			trim_right(str, chrs);
			trim_left(str, chrs);
			return str;
		}

		ustring & replace_all(ustring & str, const ustring & from, const ustring & to)
		{
			ustring::size_type pos;
			while ((pos = str.find(from)) != ustring::npos) {
				str.replace(pos, from.size(), to);
			}
			return str;
		}

		ustring & to_lower(ustring & inout)
		{
			if (!inout.empty()) {
				::CharLowerW((wchar_t*)inout.c_str());
			}
			return inout;
		}

		ustring & to_upper(ustring & inout)
		{
			if (!inout.empty()) {
				::CharUpperW((wchar_t*)inout.c_str());
			}
			return inout;
		}

		ustring & add_word(ustring & inout, const ustring & add)
		{
			return add_word(inout, add, ustring());
		}

		ustring & add_word(ustring & inout, const ustring & add, const ustring & delim)
		{
			// добаваляет строку через разделитель кроме случаев
			// 1) исходная строка пуста
			// 2) если добавляемая строка пуста
			// 3) если разделитель есть в конце исходной строки
			// 4) если разделитель есть в начале добавляемой
			ustring::size_type pos = inout.size() - delim.size();
			if (!(add.empty() || delim.empty() || inout.empty() || (inout.rfind(delim) == pos) || (add.find(delim) == 0)))
				inout += delim;
			if (!add.empty())
				inout += add;
			return inout;
		}

	}

}
