#include <basis/sys/path.hpp>
#include <basis/sys/cstr.hpp>
#include <basis/sys/sstr.hpp>

#include <basis/std/string>

namespace path {

	namespace filename {

		bool is_valid(const ustring & name)
		{
			return is_valid(name.c_str());
		}

	}

}
