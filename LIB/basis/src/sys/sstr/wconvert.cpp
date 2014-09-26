#include <basis/sys/cstr.hpp>
#include <basis/sys/sstr.hpp>

#include <basis/simstd/string>

namespace sstr {

	ustring cp2w(const char * in, UINT cp)
	{
		memory::auto_array<wchar_t> buf(cstr::convert(in, cp));
		cstr::convert(buf.data(), buf.size(), in, cp);
		return ustring(buf.data());
	}

}
