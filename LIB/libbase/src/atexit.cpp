#include <libbase/std.hpp>
#include <libbase/atexit.hpp>
#include <libbase/console.hpp>

#include <atomic>

extern "C" const Base::CrtFunction __CTOR_LIST__[1];
extern "C" const Base::CrtFunction __DTOR_LIST__[1];

namespace Base {

	const ssize_t MAX_ATEXITLIST_ENTRIES = 32;

	std::atomic<ssize_t> atexit_index(0);

	CrtFunction pf_atexitlist[MAX_ATEXITLIST_ENTRIES];

	void invoke_crt_functions(const CrtFunction * pf)
	{
		for (size_t i = 0; *pf; ++i, ++pf) {
			Base::Console::printf(L"%S:%d, pf: %p\n", __PRETTY_FUNCTION__, __LINE__, *pf);
			if (reinterpret_cast<intptr_t>(*pf) == static_cast<intptr_t>(-1)) {
				continue;
			} else {
				(*pf)();
			}
		}
	}

	void invoke_ctors()
	{
		Base::Console::printf(L"%S:%d, __CTOR_LIST__: %p\n", __PRETTY_FUNCTION__, __LINE__, __CTOR_LIST__);
		invoke_crt_functions(__CTOR_LIST__);
	}

	void invoke_dtors()
	{
		Base::Console::printf(L"%S:%d, __DTOR_LIST__: %p\n", __PRETTY_FUNCTION__, __LINE__, __DTOR_LIST__);
		invoke_crt_functions(__DTOR_LIST__);
	}

	void init_atexit()
	{
		atexit(&invoke_dtors);

		invoke_ctors();
	}

	void invoke_atexit()
	{
		ssize_t i = ((atexit_index >= MAX_ATEXITLIST_ENTRIES) ? MAX_ATEXITLIST_ENTRIES : static_cast<ssize_t>(atexit_index)) - 1;

		Base::Console::printf(L"%S:%d, atexit_index: %Id, ind: %Id\n", __PRETTY_FUNCTION__, __LINE__, ssize_t(atexit_index), i);
		for (; i >= 0; --i)
		{
			Base::Console::printf(L"%S:%d, ind: %Id, func: %p\n", __PRETTY_FUNCTION__, __LINE__, i, pf_atexitlist[i]);
			pf_atexitlist[i]();
		}
	}

	int atexit(CrtFunction pf)
	{
		ssize_t ind = atexit_index++;

		Base::Console::printf(L"%S:%d, func: %p, index: %Id\n", __PRETTY_FUNCTION__, __LINE__, pf, ind);
		if (ind < MAX_ATEXITLIST_ENTRIES)
		{
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
