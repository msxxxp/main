
#include <basis/sys/crt.hpp>
//#include <basis/sys/console.hpp>
//#include <basis/sys/memory.hpp>

#include <atomic>
#include <stdio.h>

extern "C" {
	extern const crt::Function __CTOR_LIST__[1];
	extern const crt::Function __DTOR_LIST__[1];
}

namespace {
	const ssize_t MAX_ATEXITLIST_ENTRIES = 64;

	std::atomic<ssize_t> atExitIndex(0);

	crt::Function atExitArray[MAX_ATEXITLIST_ENTRIES];
}

namespace crt {

	void invoke_crt_functions(const Function * pf, ptrdiff_t step)
	{
		TraceFunc();
		for (; *pf; pf += step) {
			if (reinterpret_cast<intptr_t>(*pf) == static_cast<intptr_t>(-1)) {
				continue;
			} else {
				TraceFuncFormat("%s:%d, %p->%p\n", __PRETTY_FUNCTION__, __LINE__, pf, *pf);
				(*pf)();
			}
		}
		TraceFunc();
	}

	void invoke_ctors()
	{
		TraceFuncFormat("%s:%d, __CTOR_LIST__: %p\n", __PRETTY_FUNCTION__, __LINE__, __CTOR_LIST__);
		invoke_crt_functions(__CTOR_LIST__, 1);
		TraceFunc();
	}

	void invoke_dtors()
	{
		TraceFuncFormat("%s:%d, __DTOR_LIST__: %p\n", __PRETTY_FUNCTION__, __LINE__, __DTOR_LIST__);
		invoke_crt_functions(__DTOR_LIST__, 1);
		TraceFunc();
	}

	void init_atexit()
	{
		TraceFunc();
		for (ssize_t i = 0; i < MAX_ATEXITLIST_ENTRIES; ++i)
			atExitArray[i] = reinterpret_cast<Function>(-1);
		atexit(reinterpret_cast<Function>(0));
		atexit(&invoke_dtors);

		invoke_ctors();
		TraceFunc();
	}

	void invoke_atexit()
	{
		TraceFunc();
		invoke_crt_functions(&atExitArray[MAX_ATEXITLIST_ENTRIES - 1], -1);
		TraceFunc();
	}

	int atexit(Function pf)
	{
		TraceFunc();
		ssize_t ind = atExitIndex++;

		TraceFuncFormat("%s:%d, func: %p, index: %d\n", __PRETTY_FUNCTION__, __LINE__, pf, (int)ind);
		if (ind < MAX_ATEXITLIST_ENTRIES)
		{
			atExitArray[ind] = pf;
			TraceFunc();
			return 0;
		}
		TraceFunc();
		return -1;
	}

	void cxa_pure_virtual()
	{
		printf("%s():%d pure virtual function called\n", __FUNCTION__, __LINE__);
//		::abort_message("pure virtual method called");
	}

}
