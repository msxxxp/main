#ifndef LIBBASE_FWD_HPP_
#define LIBBASE_FWD_HPP_

namespace Cstr {
	template<typename CharType>
	struct char_traits
	{
	};

	template<typename CharType>
	struct char_traits_ex
	{
	};
}

#if defined(NoStlString) || defined(NO_STD_STRING)
namespace Base {
	template<typename CharType, typename Traits>
	struct basic_string;
}
typedef Base::basic_string<char, Cstr::char_traits<char> > astring;
typedef Base::basic_string<wchar_t, Cstr::char_traits<wchar_t> > ustring;
#else
#ifdef _MSC_VER
	#include <string>
#else
#include <iosfwd>
#endif
typedef std::string astring;
typedef std::wstring ustring;
#endif

#endif
