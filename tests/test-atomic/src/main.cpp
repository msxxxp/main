#include <liblog/logger.hpp>
#include <libbase/std.hpp>
#include <libbase/atexit.hpp>
#include <libbase/console.hpp>
#include <libbase/lock.hpp>
#include <libbase/thread.hpp>
#include <libbase/ThreadPool.hpp>
#include <libbase/system.hpp>
#include <libbase/chrono.hpp>
#include <atomic>
#include <mutex>
#include <chrono>

namespace {
	void setup_logger()
	{
		using namespace Logger;
//		Base::Console::printf(L"%S:%d\n", __PRETTY_FUNCTION__, __LINE__);
		Default::set_level(Level::Trace);
//		Base::Console::printf(L"%S:%d\n", __PRETTY_FUNCTION__, __LINE__);
		Default::set_prefix(Prefix::Full/* | Prefix::Place*/);
//		Base::Console::printf(L"%S:%d\n", __PRETTY_FUNCTION__, __LINE__);
		Default::set_target(get_TargetToConsole());
//		Base::Console::printf(L"%S:%d\n", __PRETTY_FUNCTION__, __LINE__);
	}
}

Lock::SyncUnit_i * m1;
Lock::SyncUnit_i * m2;

struct LockMutexThead1: public Base::ThreadRoutine_i {
	size_t run(void * data) override
	{
		UNUSED(data);

		while (true) {
			Lock::lock(*m2, *m1);
//			m2->lock();
			LogTrace();
//			m1->lock();
			Sleep(33);
			m1->unlock();
			m2->unlock();
		}

		return 0;
	}
};

struct LockMutexThead2: public Base::ThreadRoutine_i {
	size_t run(void * data) override
	{
		UNUSED(data);

		while (true) {
			Lock::lock(*m1, *m2);
//			m1->lock();
			LogTrace();
//			m2->lock();
			Sleep(33);
			m1->unlock();
			m2->unlock();
		}

		return 0;
	}
};


int test_chrono()
{
	using std::chrono::duration_cast;
	using std::chrono::microseconds;
	using std::chrono::milliseconds;
	using std::chrono::nanoseconds;
	using std::chrono::steady_clock;

	for (uint64_t size = 1; size < 100000000; size *= 5) {
		auto start1= std::chrono::steady_clock::now();
		auto start2 = std::chrono::system_clock::now();
		auto start3 = Base::chrono::perfomance_clock::now();
		std::vector<int> v(size, 42);
		auto end1 = std::chrono::steady_clock::now();
		auto end2 = std::chrono::system_clock::now();
		auto end3 = Base::chrono::perfomance_clock::now();

		auto elapsed1 = end1 - start1;
		auto elapsed2 = end2 - start2;
		auto elapsed3 = end3 - start3;
		LogDebug(L"size1: %I64u, count1: %I64u\n", size, (uint64_t)duration_cast<nanoseconds>(elapsed1).count());
		LogDebug(L"size2: %I64u, count2: %I64u\n", size, (uint64_t)duration_cast<nanoseconds>(elapsed2).count());
		LogDebug(L"size3: %I64u, count3: %I64u\n", size, (uint64_t)duration_cast<nanoseconds>(elapsed3).count());
	}
	return 0;
}

int test_atomic()
{
	std::atomic_uint_fast64_t a;

	int64_t b = a;
	Base::Console::printf(L"%d\n", a.is_lock_free());

	return b;
}

int test_lock()
{
	m1 = Lock::get_CritSection();
	m2 = Lock::get_CritSection();

	LockMutexThead1 routine1;
	LockMutexThead2 routine2;

	Base::ThreadPool threads;
	threads.create_thread(&routine1);
	threads.create_thread(&routine2);

	threads.wait_all();

//	Lock::lock(m1, m2);

	return 0;
}


int wWmain()
{
	setup_logger();

	LogTrace();
//	return test_atomic();

//	return test_lock();

	return test_chrono();
}

/// ========================================================================== Startup (entry point)
#ifdef NDEBUG
extern "C" {
	int atexit(Base::CrtFunction pf)
	{
		return Base::atexit(pf);
	}

	void __cxa_pure_virtual(void)
	{
		Base::cxa_pure_virtual();
	}

	int mainCRTStartup()
	{
		//	int	WinMainCRTStartup() {
		Base::init_atexit();
		//		Base::Console::printf(L"%S:%d\n", __PRETTY_FUNCTION__, __LINE__);
		int Result = 0;

		//		STARTUPINFO StartupInfo = {sizeof(STARTUPINFO), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
		//		::GetStartupInfo(&StartupInfo);
		//
		//		Result = wWinMain(::GetModuleHandle(nullptr), nullptr, ::GetCommandLine(),
		//						  StartupInfo.dwFlags & STARTF_USESHOWWINDOW ? StartupInfo.wShowWindow : SW_SHOWDEFAULT);
		Result = wWmain();

		Base::invoke_atexit();
		::ExitProcess(Result);
		return Result;
	}

	//	BOOL WINAPI	DllMainCRTStartup(HANDLE, DWORD dwReason, PVOID) {
	//		switch (dwReason) {
	//			case DLL_PROCESS_ATTACH:
	//				init_atexit();
	//				break;
	//
	//			case DLL_PROCESS_DETACH:
	//				invoke_atexit();
	//				break;
	//		}
	//		return true;
	//	}
}
#endif
