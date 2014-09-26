#include <basis/sys/crt.hpp>
#include <basis/sys/console.hpp>
#include <basis/sys/memory.hpp>

#include <atomic>

extern "C" {
	extern const crt::Function __CTOR_LIST__[1];
	extern const crt::Function __DTOR_LIST__[1];
}

namespace {
	const ssize_t MAX_ATEXITLIST_ENTRIES = 32;

	std::atomic<ssize_t> atExitIndex(0);

	crt::Function atExitArray[MAX_ATEXITLIST_ENTRIES];
}

namespace crt {

	void invoke_crt_functions(const Function * pf, ptrdiff_t step)
	{
		for (; *pf; pf += step) {
			if (reinterpret_cast<intptr_t>(*pf) == static_cast<intptr_t>(-1)) {
				continue;
			} else {
//				console::printf(L"%S:%d, pf: %p\n", __PRETTY_FUNCTION__, __LINE__, *pf);
				(*pf)();
			}
		}
	}

	void invoke_ctors()
	{
//		console::printf(L"%S:%d, __CTOR_LIST__: %p\n", __PRETTY_FUNCTION__, __LINE__, __CTOR_LIST__);
		invoke_crt_functions(__CTOR_LIST__, 1);
	}

	void invoke_dtors()
	{
		console::printf(L"%S:%d, __DTOR_LIST__: %p\n", __PRETTY_FUNCTION__, __LINE__, __DTOR_LIST__);
		invoke_crt_functions(__DTOR_LIST__, 1);

//		using namespace memory::watchdog;
//		if (memory::watchdog::stop())
//		{
//			console::printf(L"wd alloc: %Iu \n", get_allocations());
//			console::printf(L"wd free : %Iu \n", get_deletions());
//			console::printf(L"wd diff : %I64d \n", get_allocations_size() - get_deletions_size());
//		}
	}

	void init_atexit()
	{
//		memory::watchdog::start();
//		console::printf(L"%S():%d\n", __FUNCTION__, __LINE__);
		for (ssize_t i = 0; i < MAX_ATEXITLIST_ENTRIES; ++i)
			atExitArray[i] = reinterpret_cast<Function>(-1);
		atexit(reinterpret_cast<Function>(0));
		atexit(&invoke_dtors);

		invoke_ctors();
	}

	void invoke_atexit()
	{
//		console::printf(L"%S():%d\n", __FUNCTION__, __LINE__);
		invoke_crt_functions(&atExitArray[MAX_ATEXITLIST_ENTRIES - 1], -1);
	}

	int atexit(Function pf)
	{
		ssize_t ind = atExitIndex++;

//		console::printf(L"%S:%d, func: %p, index: %Id\n", __PRETTY_FUNCTION__, __LINE__, pf, ind);
		if (ind < MAX_ATEXITLIST_ENTRIES)
		{
			atExitArray[ind] = pf;
			return 0;
		}
		return -1;
	}

	void cxa_pure_virtual()
	{
//		console::printf(L"%S():%d pure virtual method called\n", __FUNCTION__, __LINE__);
//		::abort_message("pure virtual method called");
	}

}
