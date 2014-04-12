#ifndef LIBBASE_CSTR_FROM_NUMBER_HPP_
#define LIBBASE_CSTR_FROM_NUMBER_HPP_

#include <system/cstr.hpp>

namespace cstr {

	inline PSTR convert_num(PSTR to, int64_t num, ssize_t base = 10)
	{
		return ::_i64toa(num, to, base); //lltoa
	}

	inline wchar_t * convert_num(wchar_t * to, int64_t num, ssize_t base = 10)
	{
		return ::_i64tow(num, to, base); //lltow
	}


}

#endif
