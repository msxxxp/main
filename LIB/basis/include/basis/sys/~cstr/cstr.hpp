#ifndef LIBBASE_CSTR_CSTR_HPP_
#define LIBBASE_CSTR_CSTR_HPP_

#include <basis/sys/memory.hpp>

namespace cstr {

	namespace inplace {
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
	CharType to_upper(CharType chr)
	{
		return cstr::char_traits<CharType>::to_upper(chr);
	}

	template<typename CharType>
	CharType to_lower(CharType chr)
	{
		return cstr::char_traits<CharType>::to_lower(chr);
	}

	inline size_t convert(const char* from, UINT cp)
	{
		return ::MultiByteToWideChar(cp, 0, from, -1, nullptr, 0);
	}

	inline size_t convert(wchar_t* to, size_t size, const char* from, UINT cp)
	{
		return ::MultiByteToWideChar(cp, 0, from, -1, to, (int)size);
	}

	inline size_t convert(const wchar_t* from, UINT cp)
	{
		return ::WideCharToMultiByte(cp, 0, from, -1, nullptr, 0, nullptr, nullptr);
	}

	inline size_t convert(char* to, size_t size, const wchar_t* from, UINT cp)
	{
		return ::WideCharToMultiByte(cp, 0, from, -1, to, (int)size, nullptr, nullptr);
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
	int compare_ci_1st_length(const CharType * str1, const CharType * str2)
	{
		return cstr::char_traits<CharType>::compare_ci(str1, str2, length(str1));
	}

	template<typename CharType>
	int compare_ci_2nd_length(const CharType * str1, const CharType * str2)
	{
		return cstr::char_traits<CharType>::compare_ci(str1, str2, length(str2));
	}

	template<typename CharType>
	int compare_ci(const CharType * str1, const CharType * str2, size_t count)
	{
		return cstr::char_traits<CharType>::compare_ci(str1, str2, count);
	}

	template<typename CharType>
	CharType * fill(CharType * str, size_t count, CharType chr)
	{
		return cstr::char_traits<CharType>::assign(str, count, chr);
	}

	template<typename CharType>
	CharType* dup(const CharType* src)
	{
		return src ? memory::dup<CharType*>(src, (length(src) + 1) * sizeof(CharType)) : nullptr;
	}

	template<typename CharType>
	CharType* dup(const CharType* src, size_t length)
	{
		return src ? memory::dup<CharType*>(src, (length + 1) * sizeof(CharType)) : nullptr;
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
	CharType * find(CharType * where, const CharType * what)
	{
		return const_cast<CharType *>(cstr::char_traits<CharType>::find(where, what));
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

	//inline char* RFind(const char* where, const char* what) {
	//	const char* last1 = where + Len(where);
	//	const char* last2 = what + Len(what);
	//	last2 = simstd::find_end(where, last1, what, last2);
	//	return (last1 == last2) ? nullptr : const_cast<char *>(last2);
	//}
	//
	//inline char* RFind(const char* where, CHAR what) {
	//	return ::strrchr(where, what);
	//}
	//
	//inline wchar_t* RFind(const wchar_t* where, const wchar_t* what) {
	//	const wchar_t* last1 = where + Len(where);
	//	const wchar_t* last2 = what + Len(what);
	//	last2 = simstd::find_end(where, last1, what, last2);
	//	return (last1 == last2) ? nullptr : const_cast<wchar_t *>(last2);
	//}
	//
	//inline wchar_t* RFind(const wchar_t* where, wchar_t what) {
	//	return ::wcsrchr(where, what);
	//}

	inline size_t span(const char* str, const char* strCharSet)
	{
		return ::strcspn(str, strCharSet);
	}

	inline size_t span(const wchar_t* str, const wchar_t* strCharSet)
	{
		return ::wcscspn(str, strCharSet);
	}

	inline char* reverse(char* in)
	{
		return ::_strrev(in);
	}

	inline wchar_t* reverse(wchar_t* in)
	{
		return ::_wcsrev(in);
	}

}

#endif
