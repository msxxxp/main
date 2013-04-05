#ifndef _LIBBASE_FWD_HPP_
#define _LIBBASE_FWD_HPP_

namespace Base {
	template<typename CharType>
	struct char_traits
	{
	};
}

#ifdef NoStlString
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
