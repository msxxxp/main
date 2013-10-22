#include <libbase/std.hpp>
#include <libbase/atexit.hpp>

#include <atomic>

namespace Base {

	const ssize_t MAX_ATEXITLIST_ENTRIES = 32;

	std::atomic<ssize_t> atexit_index(MAX_ATEXITLIST_ENTRIES - 1);

	FunctionAtExit pf_atexitlist[MAX_ATEXITLIST_ENTRIES];

	void init_atexit()
	{
	}

	void invoke_atexit()
	{
//		LogTrace();

		if (atexit_index < 0)
			atexit_index = 0;
		else
			++atexit_index;

		for (ssize_t i = atexit_index; i < MAX_ATEXITLIST_ENTRIES; ++i)
		{
//			LogDebug(L"[%I64d] ptr: %p\n", i, pf_atexitlist[i]);
			(*pf_atexitlist[i])();
		}
	}

	int atexit(FunctionAtExit pf)
	{
//		LogTrace();
		ssize_t ind = --atexit_index;
		if (ind >= 0)
		{
//			LogDebug(L"[%I64d] ptr: %p\n", ind, pf);
			pf_atexitlist[ind] = pf;
			return 0;
		}
		return -1;
	}

	void cxa_pure_virtual()
	{
//		::abort_message("pure virtual method called");
	}

}
