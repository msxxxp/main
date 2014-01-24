#include <system/fsys.hpp>
#include <system/cstr.hpp>
#include <system/string.hpp>

#include <simstd/string>

namespace fsys {

	namespace Filename {

		bool is_valid(PCWSTR name)
		{
			//		const wchar_t invalidCharsInName[] = L"\\\"<>:/|?*";
			return Cstr::compare(name, L".") && Cstr::compare(name, L"..") && Cstr::compare(name, L"...");
		}

		ustring extract(const ustring & path)
		{
			auto pos = path.find_last_of(PATH_SEPARATORS);
			return (pos != ustring::npos) ? path.substr(++pos) : ustring();
		}

	}

}
