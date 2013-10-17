#ifndef LIBBASE_FWD_HPP_
#define LIBBASE_FWD_HPP_

namespace Base {
	template<typename CharType>
	struct char_traits
	{
	};
}

#if defined(NoStlString) || defined(NO_STD_STRING)
namespace Base {
	template<typename CharType, typename Traits>
	struct basic_string;
}
typedef Base::basic_string<char, Base::char_traits<char> > astring;
typedef Base::basic_string<wchar_t, Base::char_traits<wchar_t> > ustring;
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
