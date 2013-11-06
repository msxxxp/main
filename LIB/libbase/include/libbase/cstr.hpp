#ifndef LIBBASE_CSTR_HPP_
#define LIBBASE_CSTR_HPP_

#include <libbase/std.hpp>
#include <wchar.h>

#ifndef _MSC_VER
extern "C" {
	long long __MINGW_NOTHROW wcstoll(const wchar_t * __restrict__, wchar_t** __restrict__, int);
	unsigned long long __MINGW_NOTHROW wcstoull(const wchar_t * __restrict__, wchar_t ** __restrict__, int);
}
#endif

namespace Base {

	ssize_t get_type_of_char(char in);

	ssize_t get_type_of_char(wchar_t in);

	template<typename CharType>
	struct Char_types
	{
		typedef uint32_t   int_type;
		//	typedef std::streampos  pos_type;
		//	typedef std::streamoff  off_type;
		//	typedef mbstate_t  state_type;
	};

	template<>
	struct char_traits<char>
	{
		typedef char              char_type;
		typedef int               int_type;
		//	typedef streampos         pos_type;
		//	typedef streamoff         off_type;
		//	typedef mbstate_t         state_type;

		static void assign(char_type & c1, const char_type & c2) noexcept {c1 = c2;}

		static bool eq(const char_type & c1, const char_type & c2) noexcept {return c1 == c2;}

		static bool lt(const char_type & c1, const char_type & c2) noexcept {return c1 < c2;}

		static int compare(const char_type * s1, const char_type* s2, size_t n) {return memcmp(s1, s2, n);}

		static size_t length(const char_type * s) {return strlen(s);}

		static const char_type * find(const char_type * s, size_t n, const char_type & a) {return (const char_type*)memchr(s, a, n);}

		static char_type * move(char_type * s1, const char_type * s2, size_t n) {return (char_type*)memmove(s1, s2, n);}

		static char_type * copy(char_type * s1, const char_type * s2, size_t n) {return (char_type*)memcpy(s1, s2, n);}

		static char_type * assign(char_type * s, size_t n, char_type a) {return (char_type*)memset(s, a, n);}

		static char_type to_char_type(const int_type & c) noexcept {return char_type(c);}

		// To keep both the byte 0xff and the eof symbol 0xffffffff
		// from ending up as 0xffffffff.
		static int_type to_int_type(const char_type & c) noexcept {return int_type(c);}

		static bool eq_int_type(const int_type & c1, const int_type & c2) noexcept {return c1 == c2;}

		static int_type eof() noexcept {return static_cast<int_type>(-1);}

		static int_type not_eof(const int_type & c) noexcept {return (c == eof()) ? 0 : c;}

		/// nonstandard
		static const char_type * find(const char_type * where, const char_type * what) {return strstr(where, what);}

		static char_type & to_upper(char_type & in) {::CharUpperBuffA(&in, 1); return in;}

		static char_type & to_lower(char_type & in) {::CharLowerBuffA(&in, 1); return in;}
	};

	template<>
	struct char_traits<wchar_t>
	{
		typedef wchar_t           char_type;
		typedef wint_t            int_type;
		//	typedef streamoff         off_type;
		//	typedef wstreampos        pos_type;
		//	typedef mbstate_t         state_type;

		static void assign(char_type & c1, const char_type & c2) noexcept {c1 = c2;}

		static bool eq(const char_type & c1, const char_type & c2) noexcept {return c1 == c2;}

		static bool lt(const char_type & c1, const char_type & c2) noexcept {return c1 < c2;}

		static int compare(const char_type * s1, const char_type* s2, size_t n) {return wmemcmp(s1, s2, n);}

		static size_t length(const char_type * s) {return wcslen(s);}

		static const char_type * find(const char_type * s, size_t n, const char_type & ch) {return wmemchr(s, ch, n);}

		static char_type * move(char_type * s1, const char_type * s2, size_t n) {return wmemmove(s1, s2, n);}

		static char_type * copy(char_type * s1, const char_type * s2, size_t n) {return wmemcpy(s1, s2, n);}

		static char_type * assign(char_type * s, size_t n, char_type a) {return wmemset(s, a, n);}

		static char_type to_char_type(const int_type & c) noexcept {return char_type(c);}

		static int_type to_int_type(const char_type & c) noexcept {return int_type(c);}

		static bool eq_int_type(const int_type & c1, const int_type & c2) noexcept {return c1 == c2;}

		static int_type eof() noexcept {return static_cast<int_type>((wint_t)(0xFFFF));}

		static int_type not_eof(const int_type & c) noexcept {return (c == eof()) ? 0 : c;}

		/// nonstandard
		static const char_type * find(const char_type * where, const char_type * what) {return wcsstr(where, what);}

		static char_type & to_upper(char_type & in) {::CharUpperBuffW(&in, 1); return in;}

		static char_type & to_lower(char_type & in) {::CharLowerBuffW(&in, 1); return in;}
	};

	namespace Inplace {
		template<typename CharType>
		CharType & to_upper(CharType & in)
		{
			return char_traits<wchar_t>::to_upper(in);
		}

		template<typename CharType>
		CharType & to_lower(CharType & in)
		{
			return char_traits<wchar_t>::to_lower(in);
		}
	}

	bool is_eol(wchar_t in);

	inline bool is_space(wchar_t in)
	{
		//	return in == L' ' || in == L'\t';
		return get_type_of_char(in) & C1_SPACE;
	}

	inline bool is_printable(wchar_t in)
	{
		return !(get_type_of_char(in) & C1_CNTRL);
	}

	inline bool is_control(wchar_t in)
	{
		//	return in == L' ' || in == L'\t';
		return get_type_of_char(in) & C1_CNTRL;
	}

	inline bool is_upper(wchar_t in)
	{
		//	return ::IsCharUpperW(in);
		return get_type_of_char(in) & C1_UPPER;
	}

	inline bool is_lower(wchar_t in)
	{
		//	return ::IsCharLowerW(in);
		return get_type_of_char(in) & C1_LOWER;
	}

	inline bool is_alpha(wchar_t in)
	{
		//	return ::IsCharAlphaW(in);
		return get_type_of_char(in) & C1_ALPHA;
	}

	inline bool is_alnum(wchar_t in)
	{
		//	return ::IsCharAlphaW(in);
		return get_type_of_char(in) & (C1_ALPHA | C1_DIGIT);
	}

	inline bool is_digit(wchar_t in)
	{
		//	return ::IsCharAlphaNumeric(in);
		return get_type_of_char(in) & C1_DIGIT;
	}

	inline bool is_xdigit(wchar_t in)
	{
		//	return ::IsCharAlphaNumeric(in);
		return get_type_of_char(in) & C1_XDIGIT;
	}

	inline bool is_punct(wchar_t in)
	{
		//	return ::IsCharAlphaNumeric(in);
		return get_type_of_char(in) & C1_PUNCT;
	}

	inline wchar_t to_upper(wchar_t in)
	{
		return Inplace::to_upper(in);
	}

	inline wchar_t to_lower(wchar_t in)
	{
		return Inplace::to_lower(in);
	}
}

namespace Cstr {

	const DWORD NORM_STOP_ON_NULL = 0x10000000;

	inline bool is_empty(PCSTR str)
	{
		return str == nullptr || *str == 0;
	}

	inline bool is_empty(PCWSTR str)
	{
		return str == nullptr || *str == 0;
	}

	inline size_t length(PCSTR in)
	{
//		return ::strlen(in);
		return ::lstrlenA(in);
	}

	inline size_t length(PCWSTR in)
	{
//		return ::wcslen(in);
		return ::lstrlenW(in);
	}

	inline int compare(PCSTR in1, PCSTR in2)
	{
		return ::strcmp(in1, in2);
	}

	inline int compare(PCSTR in1, PCSTR in2, size_t n)
	{
		return ::strncmp(in1, in2, n);
	}

	inline int compare(PCWSTR in1, PCWSTR in2)
	{
		return ::wcscmp(in1, in2);
		//	return ::wcscoll(in1, in2);
	}

	inline int compare(PCWSTR in1, PCWSTR in2, size_t n)
	{
		return ::wcsncmp(in1, in2, n);
	}

	inline int compare_cs(PCSTR in1, PCSTR in2)
	{
		return ::CompareStringA(0, SORT_STRINGSORT, in1, -1, in2, -1) - CSTR_EQUAL;
	}

	inline int compare_cs(PCSTR in1, PCSTR in2, size_t n)
	{
		return ::CompareStringA(0, NORM_STOP_ON_NULL | SORT_STRINGSORT, in1, n, in2, n) - CSTR_EQUAL;
	}

	inline int compare_cs(PCWSTR in1, PCWSTR in2)
	{
		return ::CompareStringW(0, SORT_STRINGSORT, in1, -1, in2, -1) - CSTR_EQUAL;
	}

	inline int compare_cs(PCWSTR in1, PCWSTR in2, size_t n)
	{
		return ::CompareStringW(0, NORM_STOP_ON_NULL | SORT_STRINGSORT, in1, n, in2, n) - CSTR_EQUAL;
	}

	inline int compare_ci(PCSTR in1, PCSTR in2)
	{
		//	return ::_stricmp(in1, in2);
		return ::CompareStringA(0, NORM_IGNORECASE | SORT_STRINGSORT, in1, -1, in2, -1) - CSTR_EQUAL;
	}

	inline int compare_ci(PCSTR in1, PCSTR in2, size_t n)
	{
		return ::CompareStringA(0, NORM_IGNORECASE | NORM_STOP_ON_NULL | SORT_STRINGSORT, in1, n, in2, n) - CSTR_EQUAL;
	}

	inline int compare_ci(PCWSTR in1, PCWSTR in2)
	{
		//	return ::_wcsicmp(in1, in2);
		//	return ::_wcsicoll(lhs.first.c_str(), rhs.first.c_str()) < 0;
		//	return fsf.LStricmp(lhs.first.c_str(), rhs.first.c_str()) < 0;
		return ::CompareStringW(0, NORM_IGNORECASE | SORT_STRINGSORT, in1, -1, in2, -1) - CSTR_EQUAL;
	}

	inline int compare_ci(PCWSTR in1, PCWSTR in2, size_t n)
	{
		return ::CompareStringW(0, NORM_IGNORECASE | NORM_STOP_ON_NULL | SORT_STRINGSORT, in1, n, in2, n) - CSTR_EQUAL;
	}

	template<typename Char>
	inline Char * dup(const Char * src)
	{
		return src ? Memory::dup<Char*>(src, (length(src) + 1) * sizeof(Char)) : nullptr;
	}

	inline PSTR copy(PSTR dest, PCSTR src)
	{
		return ::strcpy(dest, src);
	}

	inline PWSTR copy(PWSTR dest, PCWSTR src)
	{
		return ::wcscpy(dest, src);
	}

	inline PSTR copy(PSTR dest, PCSTR src, size_t length)
	{
		dest[length] = 0;
		return ::strncpy(dest, src, length);
	}

	inline PWSTR copy(PWSTR dest, PCWSTR src, size_t length)
	{
		dest[length] = 0;
		return ::wcsncpy(dest, src, length);
	}

	inline PSTR cat(PSTR dest, PCSTR src)
	{
		return ::strcat(dest, src);
	}

	inline PWSTR cat(PWSTR dest, PCWSTR src)
	{
		return ::wcscat(dest, src);
	}

	inline PSTR cat(PSTR dest, PCSTR src, size_t length)
	{
		return ::strncat(dest, src, length);
	}

	inline PWSTR cat(PWSTR dest, PCWSTR src, size_t length)
	{
		return ::wcsncat(dest, src, length);
	}

	inline PSTR find(PCSTR where, PCSTR what)
	{
		return (PSTR)::strstr(where, what);
	}

	inline PCSTR find(PCSTR where, char what)
	{
		return ::strchr(where, what);
	}

	inline PWSTR find(PCWSTR where, PCWSTR what)
	{
		return (PWSTR)::wcsstr(where, what);
	}

	inline PCWSTR find(PCWSTR where, wchar_t what)
	{
		return ::wcschr(where, what);
	}

	//inline PSTR RFind(PCSTR where, PCSTR what) {
	//	PCSTR last1 = where + Len(where);
	//	PCSTR last2 = what + Len(what);
	//	last2 = std::find_end(where, last1, what, last2);
	//	return (last1 == last2) ? nullptr : const_cast<PSTR>(last2);
	//}
	//
	//inline PSTR RFind(PCSTR where, CHAR what) {
	//	return ::strrchr(where, what);
	//}
	//
	//inline PWSTR RFind(PCWSTR where, PCWSTR what) {
	//	PCWSTR last1 = where + Len(where);
	//	PCWSTR last2 = what + Len(what);
	//	last2 = std::find_end(where, last1, what, last2);
	//	return (last1 == last2) ? nullptr : const_cast<PWSTR>(last2);
	//}
	//
	//inline PWSTR RFind(PCWSTR where, wchar_t what) {
	//	return ::wcsrchr(where, what);
	//}

	inline size_t span(PCSTR str, PCSTR strCharSet)
	{
		return ::strcspn(str, strCharSet);
	}

	inline size_t span(PCWSTR str, PCWSTR strCharSet)
	{
		return ::wcscspn(str, strCharSet);
	}

	///=================================================================================================
	inline PWSTR to_upper(PWSTR buf, size_t len)
	{
		::CharUpperBuffW(buf, len);
		return buf;
	}

	inline PWSTR to_upper(PWSTR s1)
	{
		return to_upper(s1, length(s1));
	}

	inline PWSTR to_lower(PWSTR buf, size_t len)
	{
		::CharLowerBuffW(buf, len);
		return buf;
	}

	inline PWSTR to_lower(PWSTR s1)
	{
		return to_lower(s1, length(s1));
	}

	inline PSTR fill(PSTR in, CHAR ch)
	{
		return ::_strset(in, ch);
	}

	inline PWSTR fill(PWSTR in, wchar_t ch)
	{
		return ::_wcsset(in, ch);
	}

	inline PSTR reverse(PSTR in)
	{
		return ::_strrev(in);
	}

	inline PWSTR reverse(PWSTR in)
	{
		return ::_wcsrev(in);
	}

	//inline PWSTR AssignStr(PCWSTR src) {
	//	size_t len = Len(src) + 1;
	//	PWSTR dest;
	//	Memory::Alloc(dest, len * sizeof(wchar_t));
	//	Copy(dest, src, len);
	//	return dest;
	//}

	inline size_t convert(PCSTR from, UINT cp)
	{
		return ::MultiByteToWideChar(cp, 0, from, -1, nullptr, 0);
	}

	inline size_t convert(PWSTR to, size_t size, PCSTR from, UINT cp)
	{
		return ::MultiByteToWideChar(cp, 0, from, -1, to, (int)size);
	}

	inline size_t convert(PCWSTR from, UINT cp)
	{
		return ::WideCharToMultiByte(cp, 0, from, -1, nullptr, 0, nullptr, nullptr);
	}

	inline size_t convert(PSTR to, size_t size, PCWSTR from, UINT cp)
	{
		return ::WideCharToMultiByte(cp, 0, from, -1, to, (int)size, nullptr, nullptr);
	}

	///=========================================================================================
	inline PSTR convert_num(PSTR to, int64_t num, ssize_t base = 10)
	{
		return ::_i64toa(num, to, base); //lltoa
	}

	inline PWSTR convert_num(PWSTR to, int64_t num, ssize_t base = 10)
	{
		return ::_i64tow(num, to, base); //lltow
	}

	///=========================================================================================
	inline int64_t to_int64(PCSTR in)
	{
		return _atoi64(in);
	}

	inline uint32_t to_uint32(PCSTR in, int base = 10)
	{
		PSTR end_ptr;
		return ::strtoul(in, &end_ptr, base);
	}

	inline int32_t to_int32(PCSTR in, int base = 10)
	{
		PSTR end_ptr;
		return ::strtol(in, &end_ptr, base);
	}

	inline double to_double(PCSTR in)
	{
		PSTR end_ptr;
		return ::strtod(in, &end_ptr);
	}

	inline uint64_t to_uint64(PCWSTR in, int base = 10)
	{
#ifdef _MSC_VER
		return _wtoi64(in);
#else
		PWSTR end_ptr;
		return ::wcstoull(in, &end_ptr, base);
#endif
	}

	inline int64_t to_int64(PCWSTR in, int base = 10)
	{
#ifdef _MSC_VER
		return _wtoi64(in);
#else
		PWSTR end_ptr;
		return ::wcstoll(in, &end_ptr, base);
#endif
	}

	inline uint32_t to_uint32(PCWSTR in, int base = 10)
	{
		PWSTR end_ptr;
		return ::wcstoul(in, &end_ptr, base);
	}

	inline int32_t to_int32(PCWSTR in, int base = 10)
	{
		PWSTR end_ptr;
		return ::wcstol(in, &end_ptr, base);
	}

	inline double to_double(PCWSTR in)
	{
		PWSTR end_ptr;
		return ::wcstod(in, &end_ptr);
	}

	///================================================================================= NamedValues
	template<typename Type>
	struct NamedValues {
		PCWSTR name;
		Type value;

		static PCWSTR GetName(const NamedValues<Type> dim[], size_t size, const Type & in)
		{
			for (size_t i = 0; i < size; ++i) {
				if (dim[i].value == in) {
					return dim[i].name;
				}
			}
			return L"unknown";
		}

		static Type GetValue(const NamedValues<Type> dim[], size_t size, PCWSTR name)
		{
			for (size_t i = 0; i < size; ++i) {
				if (Cstr::compare(dim[i].name, name)) {
					return dim[i].value;
				}
			}
			return 0;
		}
	};
}

#endif
