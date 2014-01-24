#include <system/configure.hpp>
#include <system/cstr.hpp>
#include <system/string.hpp>
#include <simstd/string>

namespace String {

	simstd::string w2cp(const wchar_t * in, UINT cp)
	{
		memory::auto_array<CHAR> buf(Cstr::convert(in, cp));
		Cstr::convert(buf.data(), buf.size(), in, cp);
		return simstd::string(&buf[0]);
	}

	simstd::wstring cp2w(const char * in, UINT cp)
	{
		memory::auto_array<wchar_t> buf(Cstr::convert(in, cp));
		Cstr::convert(buf.data(), buf.size(), in, cp);
		return simstd::wstring(buf.data());
	}

}
