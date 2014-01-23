#ifndef LIBSTL_SYSTEM_WIN64_HPP_
#define LIBSTL_SYSTEM_WIN64_HPP_

#include <system/configure.hpp>

namespace win64 {

	bool is_WOW64();

	bool disable_WOW64(PVOID & oldValue);

	bool enable_WOW64(PVOID & oldValue);

}

#endif
