#include <libbase/std.hpp>
#include <libbase/atexit.hpp>
#include <libbase/console.hpp>

#include <atomic>

extern "C" const Base::CrtFunction __CTOR_LIST__[1];
extern "C" const Base::CrtFunction __DTOR_LIST__[1];

namespace Base {

	const ssize_t MAX_ATEXITLIST_ENTRIES = 32;

	std::atomic<ssize_t> atExitIndex(0);

	CrtFunction atExitArray[MAX_ATEXITLIST_ENTRIES];

	void invoke_crt_functions(const CrtFunction * pf, ptrdiff_t step)
	{
		for (; *pf; pf += step) {
			if (reinterpret_cast<intptr_t>(*pf) == static_cast<intptr_t>(-1)) {
				continue;
			} else {
				Base::Console::printf(L"%S:%d, pf: %p\n", __PRETTY_FUNCTION__, __LINE__, *pf);
				(*pf)();
			}
		}
	}

	void invoke_ctors()
	{
		Base::Console::printf(L"%S:%d, __CTOR_LIST__: %p\n", __PRETTY_FUNCTION__, __LINE__, __CTOR_LIST__);
		invoke_crt_functions(__CTOR_LIST__, 1);
	}

	void invoke_dtors()
	{
		Base::Console::printf(L"%S:%d, __DTOR_LIST__: %p\n", __PRETTY_FUNCTION__, __LINE__, __DTOR_LIST__);
		invoke_crt_functions(__DTOR_LIST__, 1);
	}

	void init_atexit()
	{
		Base::Console::printf(L"%S():%d\n", __FUNCTION__, __LINE__);
		for (ssize_t i = 0; i < MAX_ATEXITLIST_ENTRIES; ++i)
			atExitArray[i] = reinterpret_cast<CrtFunction>(-1);
		atexit(reinterpret_cast<CrtFunction>(0));
		atexit(&invoke_dtors);

		invoke_ctors();
	}

	void invoke_atexit()
	{
		Base::Console::printf(L"%S():%d\n", __FUNCTION__, __LINE__);
		invoke_crt_functions(&atExitArray[MAX_ATEXITLIST_ENTRIES - 1], -1);
	}

	int atexit(CrtFunction pf)
	{
		ssize_t ind = atExitIndex++;

		Base::Console::printf(L"%S:%d, func: %p, index: %Id\n", __PRETTY_FUNCTION__, __LINE__, pf, ind);
		if (ind < MAX_ATEXITLIST_ENTRIES)
		{
			atExitArray[ind] = pf;
			return 0;
		}
		return -1;
	}

	void cxa_pure_virtual()
	{
		Base::Console::printf(L"%S():%d\n", __FUNCTION__, __LINE__);
//		::abort_message("pure virtual method called");
	}

}
