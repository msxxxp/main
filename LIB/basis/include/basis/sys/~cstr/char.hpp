#ifndef LIBBASE_CSTR_CHAR_HPP_
#define LIBBASE_CSTR_CHAR_HPP_

namespace cstr {

	template<typename CharType>
	bool is_space(CharType chr)
	{
		return cstr::char_traits<CharType>::get_type_of_char(chr) & C1_SPACE;
	}

	template<typename CharType>
	bool is_printable(CharType chr)
	{
		return !(cstr::char_traits<CharType>::get_type_of_char(chr) & C1_CNTRL);
	}

	template<typename CharType>
	bool is_control(CharType chr)
	{
		return cstr::char_traits<CharType>::get_type_of_char(chr) & C1_CNTRL;
	}

	template<typename CharType>
	bool is_upper(CharType chr)
	{
		return cstr::char_traits<CharType>::get_type_of_char(chr) & C1_UPPER;
	}

	template<typename CharType>
	bool is_lower(CharType chr)
	{
		return cstr::char_traits<CharType>::get_type_of_char(chr) & C1_LOWER;
	}

	template<typename CharType>
	bool is_alpha(CharType chr)
	{
		return cstr::char_traits<CharType>::get_type_of_char(chr) & C1_ALPHA;
	}

	template<typename CharType>
	bool is_alnum(CharType chr)
	{
		return cstr::char_traits<CharType>::get_type_of_char(chr) & (C1_ALPHA | C1_DIGIT);
	}

	template<typename CharType>
	bool is_digit(CharType chr)
	{
		return cstr::char_traits<CharType>::get_type_of_char(chr) & C1_DIGIT;
	}

	template<typename CharType>
	bool is_xdigit(CharType chr)
	{
		return cstr::char_traits<CharType>::get_type_of_char(chr) & C1_XDIGIT;
	}

	template<typename CharType>
	bool is_punct(CharType chr)
	{
		return cstr::char_traits<CharType>::get_type_of_char(chr) & C1_PUNCT;
	}

}

#endif
