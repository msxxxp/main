#ifndef _LIBBASE_CHAR_HPP_
#define _LIBBASE_CHAR_HPP_

#include <libbase/std.hpp>
#include <wchar.h>

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

#endif
