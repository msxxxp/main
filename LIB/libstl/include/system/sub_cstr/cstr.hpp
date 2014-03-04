#ifndef LIBBASE_CSTR_CSTR_HPP_
#define LIBBASE_CSTR_CSTR_HPP_

#include <system/cstr.hpp>

#include <system/memory.hpp>

namespace cstr {

	namespace Inplace {
		template<typename CharType>
		CharType & to_upper(CharType & chr)
		{
			return cstr::char_traits<CharType>::to_upper(chr);
		}

		template<typename CharType>
		CharType * to_upper(CharType * str, size_t count)
		{
			return cstr::char_traits<CharType>::to_upper(str, count);
		}

		template<typename CharType>
		CharType * to_upper(CharType * str)
		{
			return cstr::char_traits<CharType>::to_upper(str, cstr::char_traits<CharType>::length(str));
		}

		template<typename CharType>
		CharType & to_lower(CharType & chr)
		{
			return cstr::char_traits<CharType>::to_lower(chr);
		}

		template<typename CharType>
		CharType * to_lower(CharType * str, size_t count)
		{
			return cstr::char_traits<CharType>::to_lower(str, count);
		}

		template<typename CharType>
		CharType * to_lower(CharType * str)
		{
			return cstr::char_traits<CharType>::to_lower(str, cstr::char_traits<CharType>::length(str));
		}
	}

	template<typename CharType>
	bool is_empty(const CharType * str)
	{
		return cstr::char_traits<CharType>::is_empty(str);
	}

	template<typename CharType>
	size_t length(const CharType * str)
	{
		return cstr::char_traits<CharType>::length(str);
	}

	template<typename CharType>
	int compare(const CharType * str1, const CharType * str2)
	{
		return cstr::char_traits<CharType>::compare(str1, str2);
	}

	template<typename CharType>
	int compare(const CharType * str1, const CharType * str2, size_t count)
	{
		return cstr::char_traits<CharType>::compare(str1, str2, count);
	}

	template<typename CharType>
	int compare_cs(const CharType * str1, const CharType * str2)
	{
		return cstr::char_traits<CharType>::compare_cs(str1, str2);
	}

	template<typename CharType>
	int compare_cs(const CharType * str1, const CharType * str2, size_t count)
	{
		return cstr::char_traits<CharType>::compare_cs(str1, str2, count);
	}

	template<typename CharType>
	int compare_ci(const CharType * str1, const CharType * str2)
	{
		return cstr::char_traits<CharType>::compare_ci(str1, str2);
	}

	template<typename CharType>
	int compare_ci(const CharType * str1, const CharType * str2, size_t count)
	{
		return cstr::char_traits<CharType>::compare_ci(str1, str2, count);
	}

	template<typename CharType>
	CharType * dup(const CharType * src)
	{
		return src ? memory::dup<CharType*>(src, (length(src) + 1) * sizeof(CharType)) : nullptr;
	}

	template<typename CharType>
	CharType * copy(CharType * dest, const CharType * src)
	{
		return cstr::char_traits<CharType>::copy(dest, src);
	}

	template<typename CharType>
	CharType * copy(CharType * dest, const CharType * src, size_t count)
	{
		return cstr::char_traits<CharType>::ncopy(dest, src, count);
	}

	template<typename CharType>
	CharType * cat(CharType * dest, const CharType * src)
	{
		return cstr::char_traits<CharType>::cat(dest, src);
	}

	template<typename CharType>
	CharType * cat(CharType * dest, const CharType * src, size_t count)
	{
		return cstr::char_traits<CharType>::cat(dest, src, count);
	}

	template<typename CharType>
	const CharType * find(const CharType * where, const CharType * what)
	{
		return cstr::char_traits<CharType>::find(where, what);
	}

	template<typename CharType>
	const CharType * find(const CharType * where, CharType what)
	{
		return cstr::char_traits<CharType>::find(where, what);
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
		return cstr::char_traits<CharType>::to_upper(chr);
	}

	template<typename CharType>
	CharType to_lower(CharType chr)
	{
		return cstr::char_traits<CharType>::to_lower(chr);
	}

	template<typename CharType>
	CharType * fill(CharType * str, size_t count, CharType chr)
	{
		return cstr::char_traits<CharType>::assign(str, count, chr);
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

}

#endif
