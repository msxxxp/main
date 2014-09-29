#ifndef BASIS_SYS_CRT_ATEXIT_HPP_
#define BASIS_SYS_CRT_ATEXIT_HPP_

namespace crt {

	typedef void (*Function)(void);

	void init_atexit();

	void invoke_atexit();

	int atexit(Function pf);

	void cxa_pure_virtual();

}

#endif
