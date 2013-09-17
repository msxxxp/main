#ifndef SARALIB_STL_STRING_CHAR_TRAITS_HPP_
#define SARALIB_STL_STRING_CHAR_TRAITS_HPP_

#include "impl_types.hpp"
#include <string.h>

namespace sarastd
{
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

		static void assign(char_type & c1, const char_type & c2) {c1 = c2;}

		static bool eq(const char_type & c1, const char_type & c2) {return c1 == c2;}

		static bool lt(const char_type & c1, const char_type & c2) {return c1 < c2;}

		static int compare(const char_type * s1, const char_type* s2, size_t n) {return memcmp(s1, s2, n);}

		static size_t length(const char_type * s) {return strlen(s);}

		static const char_type * find(const char_type * s, size_t n, const char_type & a) {return (const char_type*)memchr(s, a, n);}

		static char_type * move(char_type * dest, const char_type * src, size_t count) {return (char_type*)memmove(dest, src, count);}

		static char_type * copy(char_type * s1, const char_type * s2, size_t n) {return (char_type*)memcpy(s1, s2, n);}

		static char_type * assign(char_type * s, size_t n, char_type a) {return (char_type*)memset(s, a, n);}

		static char_type to_char_type(const int_type & c) {return char_type(c);}

		// To keep both the byte 0xff and the eof symbol 0xffffffff
		// from ending up as 0xffffffff.
		static int_type to_int_type(const char_type & c) {return int_type(c);}

		static bool eq_int_type(const int_type & c1, const int_type & c2) {return c1 == c2;}

		static int_type eof() {return static_cast<int_type>(-1);}

		static int_type not_eof(const int_type & c) {return (c == eof()) ? 0 : c;}

		/// nonstandard
		static const char_type * find(const char_type * where, const char_type * what) {return strstr(where, what);}

//		static char_type & to_upper(char_type & in) {::CharUpperBuffA(&in, 1); return in;}
//
//		static char_type & to_lower(char_type & in) {::CharLowerBuffA(&in, 1); return in;}
	};

//	template<>
//	struct char_traits<wchar_t>
//	{
//		typedef wchar_t           char_type;
//		typedef wint_t            int_type;
//		//	typedef streamoff         off_type;
//		//	typedef wstreampos        pos_type;
//		//	typedef mbstate_t         state_type;
//
//		static void assign(char_type & c1, const char_type & c2) {c1 = c2;}
//
//		static bool eq(const char_type & c1, const char_type & c2) {return c1 == c2;}
//
//		static bool lt(const char_type & c1, const char_type & c2) {return c1 < c2;}
//
//		static int compare(const char_type * s1, const char_type* s2, size_t n) {return wmemcmp(s1, s2, n);}
//
//		static size_t length(const char_type * s) {return wcslen(s);}
//
//		static const char_type * find(const char_type * s, size_t n, const char_type & ch) {return wmemchr(s, ch, n);}
//
//		static char_type * move(char_type * s1, const char_type * s2, size_t n) {return wmemmove(s1, s2, n);}
//
//		static char_type * copy(char_type * s1, const char_type * s2, size_t n) {return wmemcpy(s1, s2, n);}
//
//		static char_type * assign(char_type * s, size_t n, char_type a) {return wmemset(s, a, n);}
//
//		static char_type to_char_type(const int_type & c) {return char_type(c);}
//
//		static int_type to_int_type(const char_type & c) {return int_type(c);}
//
//		static bool eq_int_type(const int_type & c1, const int_type & c2) {return c1 == c2;}
//
//		static int_type eof() {return static_cast<int_type>((wint_t)(0xFFFF));}
//
//		static int_type not_eof(const int_type & c) {return (c == eof()) ? 0 : c;}
//
//		/// nonstandard
//		static const char_type * find(const char_type * where, const char_type * what) {return wcsstr(where, what);}
//
//		static char_type & to_upper(char_type & in) {::CharUpperBuffW(&in, 1); return in;}
//
//		static char_type & to_lower(char_type & in) {::CharLowerBuffW(&in, 1); return in;}
//	};
}

#endif
