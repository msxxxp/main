#include <libbase/std.hpp>
#include <libbase/path.hpp>
#include <libbase/string.hpp>

namespace Base {
	namespace Filename {
		bool is_valid(PCWSTR name)
		{
			//		const wchar_t invalidCharsInName[] = L"\\\"<>:/|?*";
			return Str::compare(name, L".") && Str::compare(name, L"..") && Str::compare(name, L"...");
		}

		ustring extract(const ustring & path)
		{
			auto pos = path.find_last_of(PATH_SEPARATORS);
			return (pos != ustring::npos) ? path.substr(++pos) : ustring();
		}

	}
}
