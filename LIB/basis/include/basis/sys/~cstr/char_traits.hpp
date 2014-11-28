#ifndef LIBBASE_CSTR_CHAR_TRAITS_HPP_
#define LIBBASE_CSTR_CHAR_TRAITS_HPP_

#include <basis/simstd/~string/char_traits.hpp>

namespace cstr {

	namespace {
		const DWORD NORM_STOP_ON_NULL = 0x10000000;
	}

	template<typename CharType>
	struct char_traits
	{
	};

	template<>
	struct char_traits<char>: public simstd::char_traits<char>
	{
		typedef char              char_type;
		typedef int               int_type;
//		typedef streampos         pos_type;
//		typedef streamoff         off_type;
//		typedef mbstate_t         state_type;

		/// nonstandard
		static bool is_eol(char_type ch) {return ch == '\r' || ch == '\n';}

		static bool is_empty(const char_type * str) {return str == nullptr || *str == static_cast<char_type>(0);}

		static int compare(const char_type * str1, const char_type * str2) {return ::lstrcmpA(str1, str2);}

		static int compare_cs(const char_type * str1, const char_type * str2) {return ::CompareStringA(0, SORT_STRINGSORT, str1, -1, str2, -1) - CSTR_EQUAL;}

		static int compare_cs(const char_type * str1, const char_type * str2, size_t count) {return ::CompareStringA(0, NORM_STOP_ON_NULL | SORT_STRINGSORT, str1, count, str2, count) - CSTR_EQUAL;}

		static int compare_ci(const char_type * str1, const char_type * str2) {return ::CompareStringA(0, NORM_IGNORECASE | SORT_STRINGSORT, str1, -1, str2, -1) - CSTR_EQUAL;}

		static int compare_ci(const char_type * str1, const char_type * str2, size_t count) {return ::CompareStringA(0, NORM_IGNORECASE | NORM_STOP_ON_NULL | SORT_STRINGSORT, str1, count, str2, count) - CSTR_EQUAL;}

		static const char_type * find(const char_type * where, const char_type * what) {return strstr(where, what);}

		static const char_type * find(const char_type * where, char_type what) {return ::strchr(where, what);}

		static char_type * ncopy(char_type * dest, const char_type * src, size_t bufsize) {return dest[--bufsize] = static_cast<char_type>(0), ::strncpy(dest, src, bufsize);}

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
	struct char_traits<wchar_t>: public simstd::char_traits<wchar_t>
	{
		typedef wchar_t           char_type;
		typedef wint_t            int_type;
//		typedef streamoff         off_type;
//		typedef wstreampos        pos_type;
//		typedef mbstate_t         state_type;

		/// nonstandard
		static bool is_eol(char_type ch) {return ch == L'\r' || ch == L'\n';}

		static bool is_empty(const char_type * str) {return str == nullptr || *str == static_cast<char_type>(0);}

		static int compare(const char_type * str1, const char_type * str2) {return ::lstrcmpW(str1, str2);}

		static int compare_cs(const char_type * str1, const char_type * str2) {return ::CompareStringW(0, SORT_STRINGSORT, str1, -1, str2, -1) - CSTR_EQUAL;}

		static int compare_cs(const char_type * str1, const char_type * str2, size_t count) {return ::CompareStringW(0, NORM_STOP_ON_NULL | SORT_STRINGSORT, str1, count, str2, count) - CSTR_EQUAL;}

		static int compare_ci(const char_type * str1, const char_type * str2) {return ::CompareStringW(0, NORM_IGNORECASE | SORT_STRINGSORT, str1, -1, str2, -1) - CSTR_EQUAL;}

		static int compare_ci(const char_type * str1, const char_type * str2, size_t count) {return ::CompareStringW(0, NORM_IGNORECASE | NORM_STOP_ON_NULL | SORT_STRINGSORT, str1, count, str2, count) - CSTR_EQUAL;}

		static const char_type * find(const char_type * where, const char_type * what) {return wcsstr(where, what);}

		static const char_type * find(const char_type * where, char_type what) {return ::wcschr(where, what);}

		static char_type * ncopy(char_type * dest, const char_type * src, size_t bufsize) {return dest[--bufsize] = static_cast<char_type>(0), ::wcsncpy(dest, src, bufsize);}

		static char_type * copy(char_type * dest, const char_type * src) {return ::wcscpy(dest, src);}

		static char_type * cat(char_type * dest, const char_type * src) {return ::wcscat(dest, src);}

		static char_type * cat(char_type * dest, const char_type * src, size_t count) {return ::wcsncat(dest, src, count);}

		static char_type & to_upper(char_type & chr) {::CharUpperBuffW(&chr, 1); return chr;}

		static char_type * to_upper(char_type * str, size_t count) {::CharUpperBuffW(str, count); return str;}

		static char_type & to_lower(char_type & chr) {::CharLowerBuffW(&chr, 1); return chr;}

		static char_type * to_lower(char_type * str, size_t count) {::CharLowerBuffW(str, count); return str;}

		ssize_t get_type_of_char(char_type in) {WORD Result[2] = {0}; ::GetStringTypeW(CT_CTYPE1, &in, 1, Result); return Result[0];}
	};

}

#endif
