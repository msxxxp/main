#include <libbase/std.hpp>
#include <libbase/atexit.hpp>

namespace Base {

	const int64_t MAX_ATEXITLIST_ENTRIES = 8;

	int64_t atexit_index = MAX_ATEXITLIST_ENTRIES - 1;

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

		for (int64_t i = atexit_index; i < MAX_ATEXITLIST_ENTRIES; ++i)
		{
//			LogDebug(L"[%I64d] ptr: %p\n", i, pf_atexitlist[i]);
			(*pf_atexitlist[i])();
		}
	}

	int atexit(FunctionAtExit pf)
	{
//		LogTrace();
		int64_t ind = ::InterlockedExchangeAdd64(&atexit_index, -1);
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
