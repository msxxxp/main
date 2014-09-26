#include <basis/sys/cstr.hpp>
#include <basis/sys/sstr.hpp>

#include <basis/simstd/string>

namespace sstr {

	astring get_trim_default_achars()
	{
		return astring(" \t\r\n");
	}

	ustring get_trim_default_uchars()
	{
		return ustring(L" \t\r\n");
	}

}
