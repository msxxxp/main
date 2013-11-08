#include <libbase/std.hpp>
#include <libbase/cstr.hpp>

namespace Base {
	bool is_eol(wchar_t in)
	{
		return in == L'\r' || in == L'\n';
	}
}
