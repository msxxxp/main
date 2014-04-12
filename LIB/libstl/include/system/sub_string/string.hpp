#ifndef LIBBASE_STRING_HPP_
#define LIBBASE_STRING_HPP_

//#include <system/cstr.hpp>
#include <system/string.hpp>
#include <system/memory.hpp>

ssize_t ustring_test();

namespace String {

	astring get_empty_astring();
	ustring get_empty_ustring();

	astring get_trim_default_achars();
	ustring get_trim_default_uchars();

	namespace Inplace {
		astring & trim_left(astring & str, const astring & chrs);
		ustring & trim_left(ustring & str, const ustring & chrs);

		astring & trim_right(astring & str, const astring & chrs);
		ustring & trim_right(ustring & str, const ustring & chrs);

		astring & trim(astring & str, const astring & chrs);
		ustring & trim(ustring & str, const ustring & chrs);

		astring & replace_all(astring & str, const astring & from, const astring & to);
		ustring & replace_all(ustring & str, const ustring & from, const ustring & to);

		ustring & to_lower(ustring & inout);

		ustring & to_upper(ustring & inout);

		astring & add_word(astring & inout, const astring & add, const astring & delim);
		ustring & add_word(ustring & inout, const ustring & add, const ustring & delim);
	}

	astring trim(const astring & str, const astring & chrs);
	ustring trim(const ustring & str, const ustring & chrs);

	ustring replace_all(const ustring & str, const ustring & from, const ustring & to);

	ustring to_lower(const ustring & str);

	ustring to_upper(const ustring & str);

	ustring GetWord(const ustring &str, wchar_t d = PATH_SEPARATOR_C);

	ustring CutWord(ustring & inout, const ustring &delim, bool delDelim = true);

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

	size_t length(const astring &in);
	size_t length(const ustring &in);

	astring to_str_a(int64_t num, int base = 10);

	ustring to_str(int64_t num, int base = 10);

	astring oem(const wchar_t * in);
	astring oem(const ustring &in);

	astring ansi(const wchar_t * in);
	astring ansi(const ustring &in);

	astring utf8(const wchar_t * in);
	astring utf8(const ustring &in);

	ustring utf16(const char * in, UINT cp = CP_UTF8);
	ustring utf16(const astring &in, UINT cp = CP_UTF8);
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
//	//	return (wchar_t *)(in + ::wcscspn(in, mask));
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
//	//	return (wchar_t *)(in + ::wcsspn(in, mask));
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

ustring to_str(memory::auto_array<BYTE> buf);

memory::auto_array<BYTE> to_hash(const ustring & str);

void to_hash(const ustring & str, PBYTE & buf, size_t & size);

#endif
