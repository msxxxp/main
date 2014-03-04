#include <system/cstr.hpp>
#include <system/string.hpp>

#include <simstd/string>

namespace String {

	astring w2cp(const wchar_t * in, UINT cp)
	{
		memory::auto_array<CHAR> buf(cstr::convert(in, cp));
		cstr::convert(buf.data(), buf.size(), in, cp);
		return astring(&buf[0]);
	}

	ustring cp2w(const char * in, UINT cp)
	{
		memory::auto_array<wchar_t> buf(cstr::convert(in, cp));
		cstr::convert(buf.data(), buf.size(), in, cp);
		return ustring(buf.data());
	}

}
