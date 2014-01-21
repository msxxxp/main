#ifndef LIBSTL_SYSTEM_CRT_
#define LIBSTL_SYSTEM_CRT_

#include <system/configure.hpp>

namespace crt {

	typedef void (*Function)(void);

	void init_atexit();

	void invoke_atexit();

	int atexit(Function pf);

	void cxa_pure_virtual();

}

#endif
