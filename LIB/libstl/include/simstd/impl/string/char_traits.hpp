#ifndef LIBSTL_STRING_CHAR_TRAITS_HPP_
#define LIBSTL_STRING_CHAR_TRAITS_HPP_

#include <simstd/types.hpp>
#include <string.h>
#include <wchar.h>
//#include <cstring>
//#include <cwchar>

namespace simstd {
	template<typename CharType>
	struct char_traits
	{
	};

	template<>
	struct char_traits<char>
	{
		typedef char char_type;
		typedef int int_type;
//		typedef streampos         pos_type;
//		typedef streamoff         off_type;
//		typedef mbstate_t         state_type;

		static void assign(char_type & c1, const char_type & c2)
		{
			c1 = c2;
		}

		static bool eq(const char_type & c1, const char_type & c2)
		{
			return c1 == c2;
		}

		static bool lt(const char_type & c1, const char_type & c2)
		{
			return c1 < c2;
		}

		static int compare(const char_type * str1, const char_type * str2, size_t count)
		{
			return memcmp(str1, str2, count);
		}

		static size_t length(const char_type * str)
		{
			return strlen(str);
		}

		static const char_type * find(const char_type * where, size_t length, const char_type & what)
		{
			return (const char_type*)memchr(where, what, length);
		}

		static char_type * move(char_type * dest, const char_type * src, size_t count)
		{
			return (char_type*)memmove(dest, src, count);
		}

		static char_type * copy(char_type * dest, const char_type * src, size_t count)
		{
			return (char_type*)memcpy(dest, src, count);
		}

		static char_type * assign(char_type * str, size_t count, char_type chr)
		{
			return (char_type*)memset(str, chr, count);
		}

		static char_type to_char_type(const int_type & c)
		{
			return static_cast<char_type>(c);
		}

		// To keep both the byte 0xff and the eof symbol 0xffffffff
		// from ending up as 0xffffffff.
		static int_type to_int_type(const char_type & c)
		{
			return static_cast<int_type>(c);
		}

		static bool eq_int_type(const int_type & c1, const int_type & c2)
		{
			return c1 == c2;
		}

		static int_type eof()
		{
			return static_cast<int_type>(-1);
		}

		static int_type not_eof(const int_type & c)
		{
			return (c == eof()) ? 0 : c;
		}
	};

	template<>
	struct char_traits<wchar_t>
	{
		typedef wchar_t char_type;
		typedef wint_t int_type;
//		typedef streamoff         off_type;
//		typedef wstreampos        pos_type;
//		typedef mbstate_t         state_type;

		static void assign(char_type & c1, const char_type & c2)
		{
			c1 = c2;
		}

		static bool eq(const char_type & c1, const char_type & c2)
		{
			return c1 == c2;
		}

		static bool lt(const char_type & c1, const char_type & c2)
		{
			return c1 < c2;
		}

		static int compare(const char_type * str1, const char_type* str2, size_t count)
		{
			return wmemcmp(str1, str2, count);
		}

		static size_t length(const char_type * str)
		{
			return wcslen(str);
		}

		static const char_type * find(const char_type * where, size_t length, const char_type & what)
		{
			return wmemchr(where, what, length);
		}

		static char_type * move(char_type * dest, const char_type * src, size_t count)
		{
			return wmemmove(dest, src, count);
		}

		static char_type * copy(char_type * dest, const char_type * src, size_t count)
		{
			return wmemcpy(dest, src, count);
		}

		static char_type * assign(char_type * str, size_t count, char_type chr)
		{
			return wmemset(str, chr, count);
		}

		static char_type to_char_type(const int_type & c)
		{
			return char_type(c);
		}

		static int_type to_int_type(const char_type & c)
		{
			return int_type(c);
		}

		static bool eq_int_type(const int_type & c1, const int_type & c2)
		{
			return c1 == c2;
		}

		static int_type eof()
		{
			return static_cast<int_type>((wint_t)(0xFFFF));
		}

		static int_type not_eof(const int_type & c)
		{
			return (c == eof()) ? 0 : c;
		}
	};

}

#endif
