#ifndef LIBBASE_ATEXIT_HPP_
#define LIBBASE_ATEXIT_HPP_

namespace Base {

	typedef void (*FunctionAtExit)(void);

	void init_atexit();

	void invoke_atexit();

	int atexit(FunctionAtExit pf);

	void cxa_pure_virtual();
}

#endif
