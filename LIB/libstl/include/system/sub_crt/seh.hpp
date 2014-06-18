#ifndef LIBSTL_SYSTEM_CRT_SEH_HPP_
#define LIBSTL_SYSTEM_CRT_SEH_HPP_

namespace crt {

	LONG WINAPI unhandled_exception_filter(PEXCEPTION_POINTERS ep);

	void set_unhandled_exception_filter();

}

#endif
