#include <basis/sys/cstr.hpp>
#include <basis/sys/sstr.hpp>

#include <basis/simstd/string>

namespace sstr {

	astring w2cp(const wchar_t* in, UINT cp)
	{
		memory::auto_array<CHAR> buf(cstr::convert(in, cp));
		cstr::convert(buf.data(), buf.size(), in, cp);
		return astring(&buf[0]);
	}

}
