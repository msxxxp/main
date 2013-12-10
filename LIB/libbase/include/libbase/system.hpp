#ifndef LIBBASE_SYSTEM_HPP_
#define LIBBASE_SYSTEM_HPP_

#include <libbase/std.hpp>

namespace Base {

	namespace Process {
		ustring get_path();
	}

	struct SecurityAttributes: SECURITY_ATTRIBUTES {
		SecurityAttributes(bool inherit = false, PVOID psd = nullptr)
		{
			nLength = sizeof(SECURITY_ATTRIBUTES);
			lpSecurityDescriptor = psd;
			bInheritHandle = inherit;
		}
	};

}

#endif
