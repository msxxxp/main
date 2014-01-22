#ifndef LIBBASE_ENV_HPP_
#define LIBBASE_ENV_HPP_

#include <libbase/std.hpp>

namespace Base {
	namespace Env {

		ustring get(PCWSTR name);

		bool set(PCWSTR name, PCWSTR val);

		bool add(PCWSTR name, PCWSTR val);

		bool del(PCWSTR name);

	}
}

#endif
