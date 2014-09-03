#include <basis/sys/path.hpp>
#include <basis/sys/cstr.hpp>

namespace path {

	namespace filename {

		bool is_valid(const wchar_t * name)
		{
			//		const wchar_t invalidCharsInName[] = L"\\\"<>:/|?*";
			return cstr::compare(name, L".") && cstr::compare(name, L"..") && cstr::compare(name, L"...");
		}

	}

}
