#ifndef LIBBASE_STRING_HPP_
#define LIBBASE_STRING_HPP_

#include <libbase/std.hpp>
#include <libbase/cstr.hpp>
#include <libbase/memory.hpp>

namespace Base {

	ssize_t ustring_test();

	namespace String {

		namespace Inplace {
			astring & trim_left(astring & str, const astring & chrs = " \t\r\n");
			ustring & trim_left(ustring & str, const ustring & chrs = L" \t\r\n");

			astring & trim_right(astring & str, const astring & chrs = " \t\r\n");
			ustring & trim_right(ustring & str, const ustring & chrs = L" \t\r\n");

			astring & trim(astring & str, const astring & chrs = " \t\r\n");
			ustring & trim(ustring & str, const ustring & chrs = L" \t\r\n");

			ustring & replace_all(ustring & str, const ustring & from, const ustring & to);

			ustring & to_lower(ustring & inout);
			ustring & to_upper(ustring & inout);

			ustring & add_word(ustring & inout, const ustring & add, const ustring & delim = ustring());
		}

		inline astring trim(const astring & str, const astring & chrs = " \t\r\n")
		{
			astring tmp(str);
			return Inplace::trim(tmp, chrs);
		}

		inline ustring trim(const ustring & str, const ustring & chrs = L" \t\r\n")
		{
			ustring tmp(str);
			return Inplace::trim(tmp, chrs);
		}

		inline ustring replace_all(const ustring & str, const ustring & from, const ustring & to)
		{
			ustring ret(str);
			return Inplace::replace_all(ret, from, to);
		}

		inline ustring to_lower(const ustring & str)
		{
			ustring ret(str);
			return Inplace::to_lower(ret);
		}

		inline ustring to_upper(const ustring & str)
		{
			ustring ret(str);
			return Inplace::to_upper(ret);
		}

		ustring GetWord(const ustring &str, wchar_t d = PATH_SEPARATOR_C);

		ustring CutWord(ustring & inout, const ustring &delim = L"\t ", bool delDelim = true);

		ustring CutWordEx(ustring & inout, const ustring & delim, bool delDelim = true);

		ustring & Add(ustring & str, const wchar_t add);

		ustring & Add(ustring & str, const ustring & add);

		ustring & Cut(ustring & str, const ustring & sub);

		bool Cut(ustring & str, intmax_t & num, int base = 10);

		astring Hash2Str(const PBYTE buf, size_t size);

		astring Hash2StrNum(const PBYTE buf, size_t size);

		bool Str2Hash(const astring & str, PVOID & hash, ULONG & size);

		UINT CheckUnicode(const PVOID buf, size_t size);

		UINT IsUTF8(const PVOID buf, size_t size);

		bool GetCP(HANDLE hFile, UINT & cp, bool bUseHeuristics = false);

		ustring copy_after_last(const ustring & in, const ustring & delim);

	}

	inline size_t length(const astring &in)
	{
		return in.size();
	}

	inline size_t length(const ustring &in)
	{
		return in.size();
	}

	inline astring to_str_a(int64_t num, int base = 10)
	{
		CHAR buf[64];
		::_i64toa(num, buf, base);
		return astring(buf);
	}

	inline ustring to_str(int64_t num, int base = 10)
	{
		wchar_t buf[64];
		::_i64tow(num, buf, base);
		return ustring(buf);
	}

	inline astring oem(PCWSTR in)
	{
		return String::w2cp(in, CP_OEMCP);
	}

	inline astring oem(const ustring &in)
	{
		return String::w2cp(in.c_str(), CP_OEMCP);
	}

	inline astring ansi(PCWSTR in)
	{
		return String::w2cp(in, CP_ACP);
	}

	inline astring ansi(const ustring &in)
	{
		return String::w2cp(in.c_str(), CP_ACP);
	}

	inline astring utf8(PCWSTR in)
	{
		return String::w2cp(in, CP_UTF8);
	}

	inline astring utf8(const ustring &in)
	{
		return String::w2cp(in.c_str(), CP_UTF8);
	}

	inline ustring utf16(PCSTR in, UINT cp = CP_UTF8)
	{
		return String::cp2w(in, cp);
	}

	inline ustring utf16(const astring &in, UINT cp = CP_UTF8)
	{
		return String::cp2w(in.c_str(), cp);
	}

	//template<typename Type>
	//void StrToCont(const ustring &src, Type dst, const ustring &delim = L" \t\n\r") {
	//	ustring::size_type start, end = 0;
	//	while ((start = src.find_first_not_of(delim, end)) != ustring::npos) {
	//		end = src.find_first_of(delim, start);
	//		dst = src.substr(start, end - start);
	//	}
	//}

	//using std::placeholders::_1;
	//using std::placeholders::_2;
	//
	//template<typename Type>
	//const Type * find_first_of(const Type * where, const Type * what) {
	//	//	return (PWSTR)(in + ::wcscspn(in, mask));
	//	using namespace std;
	//	typedef const Type * str_t;
	//	typedef str_t (*func_t)(str_t, Type);
	//	str_t last1 = &where[Len(where)];
	//	str_t pos = find_if(&where[0], last1, bind((func_t)Find, what, _1));
	//	return (last1 == pos) ? nullptr : pos;
	//}
	//
	//template<typename Type>
	//const Type * find_first_not_of(const Type * where, const Type * what) {
	//	//	return (PWSTR)(in + ::wcsspn(in, mask));
	//	using namespace std;
	//	typedef const Type * str_t;
	//	typedef str_t (*func_t)(str_t, Type);
	//	str_t last1 = &where[Len(where)];
	//	str_t pos = find_if(&where[0], last1, bind(logical_not<bool>(), bind((func_t)Find, what, _1)));
	//	return (last1 == pos) ? nullptr : pos;
	//}
	//
	//template<typename Type>
	//const Type * find_last_of(const Type * where, const Type * what) {
	//	using namespace std;
	//	typedef const Type * str_t;
	//	typedef str_t (*func_t)(str_t, Type);
	//	reverse_iterator<str_t> first1(&where[Len(where)]);
	//	reverse_iterator<str_t> last1(&where[0]);
	//	reverse_iterator<str_t> pos = find_if(first1, last1, bind((func_t)Find, what, _1));
	////	reverse_iterator<str_t> pos = find_if(first1, last1, bind1st(ptr_fun<str_t, Type, str_t>(Find), what));
	//	return (last1 == pos) ? nullptr : &(*pos);
	//}
	//
	//template<typename Type>
	//const Type * find_last_not_of(const Type * where, const Type * what) {
	//	using namespace std;
	//	typedef const Type * str_t;
	//	typedef str_t (*func_t)(str_t, Type);
	//	reverse_iterator<str_t> first1(&where[Len(where)]);
	//	reverse_iterator<str_t> last1(&where[0]);
	//	reverse_iterator<str_t> pos = find_if(first1, last1, bind(logical_not<bool>(), bind((func_t)Find, what, _1)));
	////	reverse_iterator<str_t> pos = find_if(first1, last1, not1(bind1st(ptr_fun<str_t, Type, str_t>(Find), what)));
	//	return (last1 == pos) ? nullptr : &(*pos);
	//}

	ustring to_str(const SYSTEMTIME & in, bool tolocal = true);

	ustring to_str(const FILETIME & in);

	ustring to_str(const PBYTE buf, size_t size);

	ustring to_str(auto_array<BYTE> buf);

	auto_array<BYTE> to_hash(const ustring & str);

	void to_hash(const ustring & str, PBYTE & buf, size_t & size);

}

#endif
