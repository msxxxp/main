#include <liblog/logger.hpp>
#include <libbase/std.hpp>
#include <libbase/atexit.hpp>
#include <libbase/console.hpp>
#include <libbase/lock.hpp>
#include <libbase/thread.hpp>
#include <libbase/ThreadPool.hpp>
#include <atomic>
#include <mutex>

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

Lock::CriticalSection * m1;
Lock::CriticalSection * m2;

struct LockMutexThead1: public Base::ThreadRoutine_i {
	size_t run(void * data) override
	{
		UNUSED(data);

		while (true) {
			Lock::lock(m2, m1);
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
			Lock::lock(m1, m2);
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


int test_atomic()
{
	std::atomic_uint_fast64_t a;

	int64_t b = a;
	Base::Console::printf(L"%d\n", a.is_lock_free());

	return b;
}

int test_lock()
{
	m1 = new Lock::CriticalSection;
	m2 = new Lock::CriticalSection;

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

	return test_lock();
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
