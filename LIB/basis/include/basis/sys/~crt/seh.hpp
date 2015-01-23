#ifndef BASIS_SYS_CRT_SEH_HPP_
#define BASIS_SYS_CRT_SEH_HPP_

namespace crt {

	LONG WINAPI unhandled_exception_filter(PEXCEPTION_POINTERS ep);

	void set_unhandled_exception_filter();

	LONG WINAPI vectored_exception_filter(PEXCEPTION_POINTERS ep);

	void set_vectored_exception_filter();

}

#endif
