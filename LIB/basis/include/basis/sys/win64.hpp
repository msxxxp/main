#ifndef BASIS_SYS_WIN64_HPP_
#define BASIS_SYS_WIN64_HPP_

#include <basis/configure.hpp>

namespace win64 {

	bool is_WOW64();

	bool disable_WOW64(PVOID & oldValue);

	bool enable_WOW64(PVOID & oldValue);

}

#endif
