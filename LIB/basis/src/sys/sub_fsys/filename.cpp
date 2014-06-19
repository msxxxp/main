#include <basis/sys/fsys.hpp>
#include <basis/sys/cstr.hpp>
#include <basis/sys/sstr.hpp>

#include <basis/std/string>

namespace fsys {

	namespace Filename {

		bool is_valid(const wchar_t * name)
		{
			//		const wchar_t invalidCharsInName[] = L"\\\"<>:/|?*";
			return cstr::compare(name, L".") && cstr::compare(name, L"..") && cstr::compare(name, L"...");
		}

		ustring extract(const ustring & path)
		{
			auto pos = path.find_last_of(PATH_SEPARATORS);
			return (pos != ustring::npos) ? path.substr(++pos) : ustring();
		}

	}

}
