#ifndef LIBSTL_SYSTEM_CSTR_HPP_
#define LIBSTL_SYSTEM_CSTR_HPP_

#include <system/configure.hpp>

#include <system/memory.hpp>
#include <wchar.h>

#ifndef _MSC_VER
extern "C" {
	long long __MINGW_NOTHROW wcstoll(const wchar_t * __restrict__, wchar_t** __restrict__, int);
	unsigned long long __MINGW_NOTHROW wcstoull(const wchar_t * __restrict__, wchar_t ** __restrict__, int);
}
#endif

namespace Cstr {
	namespace {
		const DWORD NORM_STOP_ON_NULL = 0x10000000;
	}

	template<typename CharType>
	struct Char_types
	{
		typedef uint32_t   int_type;
//		typedef simstd::streampos  pos_type;
//		typedef simstd::streamoff  off_type;
//		typedef mbstate_t  state_type;
	};

	template<typename CharType>
	struct char_traits
	{
	};

	template<>
	struct char_traits<char>
	{
		typedef char              char_type;
		typedef int               int_type;
//		typedef streampos         pos_type;
//		typedef streamoff         off_type;
//		typedef mbstate_t         state_type;

		static void assign(char_type & c1, const char_type & c2) noexcept {c1 = c2;}

		static bool eq(const char_type & c1, const char_type & c2) noexcept {return c1 == c2;}

		static bool lt(const char_type & c1, const char_type & c2) noexcept {return c1 < c2;}

		static int compare(const char_type * str1, const char_type* str2, size_t count) {return memcmp(str1, str2, count);}

		static size_t length(const char_type * str) {return strlen(str);}

		static const char_type * find(const char_type * where, size_t length, const char_type & what) {return (const char_type*)memchr(where, what, length);}

		static char_type * move(char_type * dest, const char_type * src, size_t count) {return (char_type*)memmove(dest, src, count);}

		static char_type * copy(char_type * dest, const char_type * src, size_t count) {return (char_type*)memcpy(dest, src, count);}

		static char_type * assign(char_type * str, size_t count, char_type chr) {return (char_type*)memset(str, chr, count);}

		static char_type to_char_type(const int_type & c) noexcept {return static_cast<char_type>(c);}

		// To keep both the byte 0xff and the eof symbol 0xffffffff
		// from ending up as 0xffffffff.
		static int_type to_int_type(const char_type & c) noexcept {return static_cast<int_type>(c);}

		static bool eq_int_type(const int_type & c1, const int_type & c2) noexcept {return c1 == c2;}

		static int_type eof() noexcept {return static_cast<int_type>(-1);}

		static int_type not_eof(const int_type & c) noexcept {return (c == eof()) ? 0 : c;}

		/// nonstandard
		static bool is_empty(const char_type * str) {return str == nullptr || *str == static_cast<char_type>(0);}

		static int compare(const char_type * str1, const char_type * str2) {return ::lstrcmpA(str1, str2);}

		static int compare_cs(const char_type * str1, const char_type * str2) {return ::CompareStringA(0, SORT_STRINGSORT, str1, -1, str2, -1) - CSTR_EQUAL;}

		static int compare_cs(const char_type * str1, const char_type * str2, size_t count) {return ::CompareStringA(0, NORM_STOP_ON_NULL | SORT_STRINGSORT, str1, count, str2, count) - CSTR_EQUAL;}

		static int compare_ci(const char_type * str1, const char_type * str2) {return ::CompareStringA(0, NORM_IGNORECASE | SORT_STRINGSORT, str1, -1, str2, -1) - CSTR_EQUAL;}

		static int compare_ci(const char_type * str1, const char_type * str2, size_t count) {return ::CompareStringA(0, NORM_IGNORECASE | NORM_STOP_ON_NULL | SORT_STRINGSORT, str1, count, str2, count) - CSTR_EQUAL;}

		static const char_type * find(const char_type * where, const char_type * what) {return strstr(where, what);}

		static const char_type * find(const char_type * where, char_type what) {return ::strchr(where, what);}

		static char_type * ncopy(char_type * dest, const char_type * src, size_t bufsize) {return ::strncpy(dest, src, bufsize);}

		static char_type * copy(char_type * dest, const char_type * src) {return ::strcpy(dest, src);}

		static char_type * cat(char_type * dest, const char_type * src) {return ::strcat(dest, src);}

		static char_type * cat(char_type * dest, const char_type * src, size_t count) {return ::strncat(dest, src, count);}

		static char_type & to_upper(char_type & chr) {::CharUpperBuffA(&chr, 1); return chr;}

		static char_type * to_upper(char_type * str, size_t count) {::CharUpperBuffA(str, count); return str;}

		static char_type & to_lower(char_type & chr) {::CharLowerBuffA(&chr, 1); return chr;}

		static char_type * to_lower(char_type * str, size_t count) {::CharLowerBuffA(str, count); return str;}

		ssize_t get_type_of_char(char_type in) {WORD Result[2] = {0}; ::GetStringTypeA(LOCALE_USER_DEFAULT, CT_CTYPE1, &in, 1, Result); return Result[0];}
	};

	template<>
	struct char_traits<wchar_t>
	{
		typedef wchar_t           char_type;
		typedef wint_t            int_type;
//		typedef streamoff         off_type;
//		typedef wstreampos        pos_type;
//		typedef mbstate_t         state_type;

		static void assign(char_type & c1, const char_type & c2) noexcept {c1 = c2;}

		static bool eq(const char_type & c1, const char_type & c2) noexcept {return c1 == c2;}

		static bool lt(const char_type & c1, const char_type & c2) noexcept {return c1 < c2;}

		static int compare(const char_type * str1, const char_type* str2, size_t count) {return ::wmemcmp(str1, str2, count);}

		static size_t length(const char_type * str) {return ::wcslen(str);}

		static const char_type * find(const char_type * s, size_t n, const char_type & ch) {return wmemchr(s, ch, n);}

		static char_type * move(char_type * dest, const char_type * src, size_t count) {return wmemmove(dest, src, count);}

		static char_type * copy(char_type * dest, const char_type * src, size_t count) {return wmemcpy(dest, src, count);}

		static char_type * assign(char_type * s, size_t n, char_type a) {return wmemset(s, a, n);}

		static char_type to_char_type(const int_type & c) noexcept {return char_type(c);}

		static int_type to_int_type(const char_type & c) noexcept {return int_type(c);}

		static bool eq_int_type(const int_type & c1, const int_type & c2) noexcept {return c1 == c2;}

		static int_type eof() noexcept {return static_cast<int_type>((wint_t)(0xFFFF));}

		static int_type not_eof(const int_type & c) noexcept {return (c == eof()) ? 0 : c;}

		/// nonstandard
		static bool is_empty(const char_type * str) {return str == nullptr || *str == static_cast<char_type>(0);}

		static int compare(const char_type * str1, const char_type * str2) {return ::lstrcmpW(str1, str2);}

		static int compare_cs(const char_type * str1, const char_type * str2) {return ::CompareStringW(0, SORT_STRINGSORT, str1, -1, str2, -1) - CSTR_EQUAL;}

		static int compare_cs(const char_type * str1, const char_type * str2, size_t count) {return ::CompareStringW(0, NORM_STOP_ON_NULL | SORT_STRINGSORT, str1, count, str2, count) - CSTR_EQUAL;}

		static int compare_ci(const char_type * str1, const char_type * str2) {return ::CompareStringW(0, NORM_IGNORECASE | SORT_STRINGSORT, str1, -1, str2, -1) - CSTR_EQUAL;}

		static int compare_ci(const char_type * str1, const char_type * str2, size_t count) {return ::CompareStringW(0, NORM_IGNORECASE | NORM_STOP_ON_NULL | SORT_STRINGSORT, str1, count, str2, count) - CSTR_EQUAL;}

		static const char_type * find(const char_type * where, const char_type * what) {return wcsstr(where, what);}

		static const char_type * find(const char_type * where, char_type what) {return ::wcschr(where, what);}

		static char_type * ncopy(char_type * dest, const char_type * src, size_t bufsize) {return ::wcsncpy(dest, src, bufsize);}

		static char_type * copy(char_type * dest, const char_type * src) {return ::wcscpy(dest, src);}

		static char_type * cat(char_type * dest, const char_type * src) {return ::wcscat(dest, src);}

		static char_type * cat(char_type * dest, const char_type * src, size_t count) {return ::wcsncat(dest, src, count);}

		static char_type & to_upper(char_type & chr) {::CharUpperBuffW(&chr, 1); return chr;}

		static char_type * to_upper(char_type * str, size_t count) {::CharUpperBuffW(str, count); return str;}

		static char_type & to_lower(char_type & chr) {::CharLowerBuffW(&chr, 1); return chr;}

		static char_type * to_lower(char_type * str, size_t count) {::CharLowerBuffW(str, count); return str;}

		ssize_t get_type_of_char(char_type in) {WORD Result[2] = {0}; ::GetStringTypeW(CT_CTYPE1, &in, 1, Result); return Result[0];}
	};

	bool is_eol(wchar_t in);

	template<typename CharType>
	bool is_space(CharType chr)
	{
		return Cstr::char_traits<CharType>::get_type_of_char(chr) & C1_SPACE;
	}

	template<typename CharType>
	bool is_printable(CharType chr)
	{
		return !(Cstr::char_traits<CharType>::get_type_of_char(chr) & C1_CNTRL);
	}

	template<typename CharType>
	bool is_control(CharType chr)
	{
		return Cstr::char_traits<CharType>::get_type_of_char(chr) & C1_CNTRL;
	}

	template<typename CharType>
	bool is_upper(CharType chr)
	{
		return Cstr::char_traits<CharType>::get_type_of_char(chr) & C1_UPPER;
	}

	template<typename CharType>
	bool is_lower(CharType chr)
	{
		return Cstr::char_traits<CharType>::get_type_of_char(chr) & C1_LOWER;
	}

	template<typename CharType>
	bool is_alpha(CharType chr)
	{
		return Cstr::char_traits<CharType>::get_type_of_char(chr) & C1_ALPHA;
	}

	template<typename CharType>
	bool is_alnum(CharType chr)
	{
		return Cstr::char_traits<CharType>::get_type_of_char(chr) & (C1_ALPHA | C1_DIGIT);
	}

	template<typename CharType>
	bool is_digit(CharType chr)
	{
		return Cstr::char_traits<CharType>::get_type_of_char(chr) & C1_DIGIT;
	}

	template<typename CharType>
	bool is_xdigit(CharType chr)
	{
		return Cstr::char_traits<CharType>::get_type_of_char(chr) & C1_XDIGIT;
	}

	template<typename CharType>
	bool is_punct(CharType chr)
	{
		return Cstr::char_traits<CharType>::get_type_of_char(chr) & C1_PUNCT;
	}

	namespace Inplace {
		template<typename CharType>
		CharType & to_upper(CharType & chr)
		{
			return Cstr::char_traits<CharType>::to_upper(chr);
		}

		template<typename CharType>
		CharType * to_upper(CharType * str, size_t count)
		{
			return Cstr::char_traits<CharType>::to_upper(str, count);
		}

		template<typename CharType>
		CharType * to_upper(CharType * str)
		{
			return Cstr::char_traits<CharType>::to_upper(str, length(str));
		}

		template<typename CharType>
		CharType & to_lower(CharType & chr)
		{
			return Cstr::char_traits<CharType>::to_lower(chr);
		}

		template<typename CharType>
		CharType * to_lower(CharType * str, size_t count)
		{
			return Cstr::char_traits<CharType>::to_lower(str, count);
		}

		template<typename CharType>
		CharType * to_lower(CharType * str)
		{
			return Cstr::char_traits<CharType>::to_lower(str, Cstr::char_traits<CharType>::length(str));
		}
	}

	template<typename CharType>
	bool is_empty(const CharType * str)
	{
		return Cstr::char_traits<CharType>::is_empty(str);
	}

	template<typename CharType>
	size_t length(const CharType * str)
	{
		return Cstr::char_traits<CharType>::length(str);
	}

	template<typename CharType>
	int compare(const CharType * str1, const CharType * str2)
	{
		return Cstr::char_traits<CharType>::compare(str1, str2);
	}

	template<typename CharType>
	int compare(const CharType * str1, const CharType * str2, size_t count)
	{
		return Cstr::char_traits<CharType>::compare(str1, str2, count);
	}

	template<typename CharType>
	int compare_cs(const CharType * str1, const CharType * str2)
	{
		return Cstr::char_traits<CharType>::compare_cs(str1, str2);
	}

	template<typename CharType>
	int compare_cs(const CharType * str1, const CharType * str2, size_t count)
	{
		return Cstr::char_traits<CharType>::compare_cs(str1, str2, count);
	}

	template<typename CharType>
	int compare_ci(const CharType * str1, const CharType * str2)
	{
		return Cstr::char_traits<CharType>::compare_ci(str1, str2);
	}

	template<typename CharType>
	int compare_ci(const CharType * str1, const CharType * str2, size_t count)
	{
		return Cstr::char_traits<CharType>::compare_ci(str1, str2, count);
	}

	template<typename CharType>
	CharType * dup(const CharType * src)
	{
		return src ? memory::dup<CharType*>(src, (length(src) + 1) * sizeof(CharType)) : nullptr;
	}

	template<typename CharType>
	CharType * copy(CharType * dest, const CharType * src)
	{
		return Cstr::char_traits<CharType>::copy(dest, src);
	}

	template<typename CharType>
	CharType * copy(CharType * dest, const CharType * src, size_t count)
	{
		return Cstr::char_traits<CharType>::ncopy(dest, src, count);
	}

	template<typename CharType>
	CharType * cat(CharType * dest, const CharType * src)
	{
		return Cstr::char_traits<CharType>::cat(dest, src);
	}

	template<typename CharType>
	CharType * cat(CharType * dest, const CharType * src, size_t count)
	{
		return Cstr::char_traits<CharType>::cat(dest, src, count);
	}

	template<typename CharType>
	const CharType * find(const CharType * where, const CharType * what)
	{
		return Cstr::char_traits<CharType>::find(where, what);
	}

	template<typename CharType>
	const CharType * find(const CharType * where, CharType what)
	{
		return Cstr::char_traits<CharType>::find(where, what);
	}

	//inline PSTR RFind(PCSTR where, PCSTR what) {
	//	PCSTR last1 = where + Len(where);
	//	PCSTR last2 = what + Len(what);
	//	last2 = simstd::find_end(where, last1, what, last2);
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
	//	last2 = simstd::find_end(where, last1, what, last2);
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

	inline PSTR reverse(PSTR in)
	{
		return ::_strrev(in);
	}

	inline PWSTR reverse(PWSTR in)
	{
		return ::_wcsrev(in);
	}

	///=============================================================================================
	template<typename CharType>
	CharType to_upper(CharType chr)
	{
		return Cstr::char_traits<CharType>::to_upper(chr);
	}

	template<typename CharType>
	CharType to_lower(CharType chr)
	{
		return Cstr::char_traits<CharType>::to_lower(chr);
	}

	template<typename CharType>
	CharType * fill(CharType * str, size_t count, CharType chr)
	{
		return Cstr::char_traits<CharType>::assign(str, count, chr);
	}

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

	///=============================================================================================
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
