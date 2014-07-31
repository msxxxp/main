#include <basis/sys/console.hpp>
#include <basis/sys/crt.hpp>
#include <basis/sys/logger.hpp>
#include <basis/sys/sync.hpp>
#include <basis/sys/thread.hpp>

#include <basis/std/chrono>
#include <basis/std/mutex>

#include <atomic>

#include <iostream>
#include <iomanip>
#include <string>
//#include <chrono>
//#include <cmath>
//#include <thread>
//#include <future>
//#include <functional>

namespace {
	void setup_logger()
	{
		LogSetOptions(L"logger:///default?level=tr;prefix=fu;target=co");
	}
}

sync::SyncUnit_i * m1;
sync::SyncUnit_i * m2;

struct LockMutexThead1: public thread::Routine {
	size_t run(void * data) override
	{
		UNUSED(data);

		while (true) {
			simstd::lock(*m2, *m1);
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

struct LockMutexThead2: public thread::Routine {
	size_t run(void * data) override
	{
		UNUSED(data);

		while (true) {
			simstd::lock(*m1, *m2);
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

template<typename V, typename R>
std::ostream& operator <<(std::ostream& s, const std::chrono::duration<V, R>& d)
{
	s << "[" << d.count() << " of " << R::num << "/" << R::den << "]";
	return s;
}

template<typename C>
void printClockData()
{
	using namespace std;

	cout << "- precision: ";

	// if time unit is less or equal one millisecond
	typedef typename C::period P; // type of time unit
	if (ratio_less_equal<P, milli>::value) {
		// convert to and print as milliseconds
		typedef typename ratio_multiply<P, kilo>::type TT;
		cout << fixed << double(TT::num) / TT::den << " milliseconds" << endl;
	} else {
		// print as seconds
		cout << fixed << double(P::num) / P::den << " seconds" << endl;
	}
	cout << "- is_steady: " << boolalpha << C::is_steady << endl;
}

std::string asString(const std::chrono::system_clock::time_point& tp)
{
	// convert to system time:
	std::time_t t = std::chrono::system_clock::to_time_t(tp);
	std::string ts = std::asctime(gmtime(&t));
//	std::string ts = std::ctime(&t); // convert to calendar time
	ts.resize(ts.size() - 1);         // skip trailing newline
	return ts;
}

int test_atomic()
{
	std::atomic_uint_fast64_t a;

	int64_t b = a;
	LogReport(L"-> %d\n", a.is_lock_free());

	return b;
}

int test_lock()
{
	m1 = sync::get_CritSection();
	m2 = sync::get_CritSection();

	LockMutexThead1 routine1;
	LockMutexThead2 routine2;

	thread::Pool threads;
	threads.create_thread(&routine1);
	threads.create_thread(&routine2);

	threads.wait_all();

	return 0;
}

int test_chrono()
{
	using std::chrono::duration_cast;
	using std::chrono::microseconds;
	using std::chrono::milliseconds;
	using std::chrono::nanoseconds;
	using std::chrono::steady_clock;

	std::cout << "system_clock: " << std::endl;
	printClockData<std::chrono::system_clock>();

	std::cout << "\nhigh_resolution_clock: " << std::endl;
	printClockData<std::chrono::high_resolution_clock>();

	std::cout << "\nsteady_clock: " << std::endl;
	printClockData<std::chrono::steady_clock>();

	std::cout << "\nperfomance_clock: " << std::endl;
	printClockData<simstd::chrono::perfomance_clock>();

	// print the epoch of this system clock:
	std::chrono::system_clock::time_point tp;
	std::cout << "epoch: " << asString(tp) << std::endl;

	// print current time:
	tp = std::chrono::system_clock::now();
	std::cout << "now:   " << asString(tp) << std::endl;

	// print maximum time of this system clock:
	tp = std::chrono::system_clock::time_point::max();
	std::cout << "max:   " << asString(tp) << std::endl;

	// print minimum time of this system clock:
	tp = std::chrono::system_clock::time_point::min();
	std::cout << "min:   " << asString(tp) << std::endl;

	for (uint64_t size = 1; size < 100000000; size *= 5) {
		auto start1 = std::chrono::steady_clock::now();
//		auto start2 = std::chrono::system_clock::now();
		auto start3 = simstd::chrono::perfomance_clock::now();
		simstd::vector<int> v(size, 42);
		auto end1 = std::chrono::steady_clock::now();
//		auto end2 = std::chrono::system_clock::now();
		auto end3 = simstd::chrono::perfomance_clock::now();

		auto elapsed1 = end1 - start1;
//		auto elapsed2 = end2 - start2;
		auto elapsed3 = end3 - start3;
		LogDebug(L"size1: %I64u, count1: %I64u, count1: %I64u\n", size, (uint64_t )duration_cast<nanoseconds>(elapsed1).count(), (uint64_t )duration_cast<milliseconds>(elapsed1).count());
//		LogDebug(L"size2: %I64u, count2: %I64u, count2: %I64u\n", size, (uint64_t)duration_cast<nanoseconds>(elapsed2).count(), (uint64_t)duration_cast<milliseconds>(elapsed2).count());
		LogDebug(L"size3: %I64u, count3: %I64u, count3: %I64u\n", size, (uint64_t )duration_cast<nanoseconds>(elapsed3).count(), (uint64_t )duration_cast<milliseconds>(elapsed3).count());
	}
	return 0;
}

//// unique function to avoid disambiguating the std::pow overload set
//int f(int x, int y) { return std::pow(x,y); }
//
//void task_lambda()
//{
//    std::packaged_task<int(int,int)> task([](int a, int b) {
//        return std::pow(a, b);
//    });
//    std::future<int> result = task.get_future();
//
//    task(2, 9);
//
//    std::cout << "task_lambda:\t" << result.get() << '\n';
//}
//
//void task_bind()
//{
//    std::packaged_task<int()> task(std::bind(f, 2, 11));
//    std::future<int> result = task.get_future();
//
//    task();
//
//    std::cout << "task_bind:\t" << result.get() << '\n';
//}
//
//void task_thread()
//{
//    std::packaged_task<int(int,int)> task(f);
//    std::future<int> result = task.get_future();
//
//    std::thread task_td(std::move(task), 2, 10);
//    task_td.join();
//
//    std::cout << "task_thread:\t" << result.get() << '\n';
//}

int WINAPI wWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, LPWSTR /*lpCmdLine*/, int /*nShowCmd*/)
{
	console::printf(L"%S:%d\n", __PRETTY_FUNCTION__, __LINE__);

	setup_logger();

	LogTrace();

	return 0;
}

extern "C" int wmain(int argc, wchar_t * argv[])
{
	setup_logger();

	LogTrace();
	LogDebug(L"argc: %d\n", argc);
	for (int i = 0; i < argc; ++i)
		LogDebug(L"%s\n", argv[i]);

	setup_logger();

	LogTrace();
//	task_lambda();
//	task_bind();
//	task_thread();

//	test_atomic();

//	test_lock();

	return test_chrono();
}

/// ========================================================================== Startup (entry point)
#ifdef NDEBUG
extern "C" {
	int mainCRTStartup()
	{
		console::printf(L"%S:%d\n", __PRETTY_FUNCTION__, __LINE__);
		crt::init_atexit();

		int argc = 0;
		wchar_t ** argv = ::CommandLineToArgvW(::GetCommandLineW(), &argc);

		int Result = wmain(argc, argv);

		::LocalFree(argv);
		crt::invoke_atexit();
		::ExitProcess(Result);
		return Result;
	}

	int WinMainCRTStartup() // -mwindows
	{
		console::printf(L"%S:%d\n", __PRETTY_FUNCTION__, __LINE__);
		crt::init_atexit();

		int ret = 0;

		STARTUPINFOW startupInfo;
		::RtlSecureZeroMemory(&startupInfo, sizeof(startupInfo));
		startupInfo.cb = sizeof(startupInfo);
		::GetStartupInfoW(&startupInfo);

		ret = wWinMain(::GetModuleHandleW(nullptr), nullptr, ::GetCommandLineW(), startupInfo.dwFlags & STARTF_USESHOWWINDOW ? startupInfo.wShowWindow : SW_SHOWDEFAULT);

		crt::invoke_atexit();
		::ExitProcess(ret);
		return ret;
	}

	BOOL WINAPI DllMainCRTStartup(HANDLE, DWORD dwReason, PVOID)
	{
		console::printf(L"%S:%d\n", __PRETTY_FUNCTION__, __LINE__);
		switch (dwReason) {
			case DLL_PROCESS_ATTACH:
			crt::init_atexit();
			break;

			case DLL_PROCESS_DETACH:
			crt::invoke_atexit();
			break;
		}
		return true;
	}

	int atexit(crt::Function pf)
	{
		return crt::atexit(pf);
	}

	void __cxa_pure_virtual(void)
	{
		crt::cxa_pure_virtual();
	}

	void _pei386_runtime_relocator()
	{
		console::printf(L"%S:%d\n", __PRETTY_FUNCTION__, __LINE__);
	}
}
#endif
