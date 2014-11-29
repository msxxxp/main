#include <basis/sys/path.hpp>
#include <basis/sys/cstr.hpp>
#include <basis/sys/sstr.hpp>

#include <basis/simstd/string>

namespace path {

	namespace filename {

		ustring extract(const ustring& path)
		{
			auto pos = path.find_last_of(PATH_SEPARATORS);
			return (pos != ustring::npos) ? path.substr(++pos) : ustring();
		}

	}

}
