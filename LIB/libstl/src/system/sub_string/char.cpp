#include <system/cstr.hpp>

namespace Cstr {
	bool is_eol(wchar_t in)
	{
		return in == L'\r' || in == L'\n';
	}
}
