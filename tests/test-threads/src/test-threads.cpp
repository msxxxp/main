#include <system/console.hpp>
#include <system/sync.hpp>
#include <system/thread.hpp>
#include <system/traceback.hpp>
//#include <libbase/messaging.hpp>
//#include <libbase/thread.hpp>
//#include <libbase/ThreadPool.hpp>
//#include <libbase/atexit.hpp>
//#include <libbase/wstr.hpp>
#include <liblog/logger.hpp>
//#include <libbase/err.hpp>

//#include <stdio.h>
//#include <functional>
//#include <memory>
//#include <vector>
//#include <libbase/window.hpp>
//
//struct CMyWnd: public Base::Window {
//	CMyWnd()
//	{
//		LogTrace();
//		add_handler(WM_CREATE, &CMyWnd::OnCreate);
//		add_handler(WM_DESTROY, &CMyWnd::OnDestroy);
//		add_handler(WM_CLOSE, &CMyWnd::OnClose);
//		add_handler(WM_ACTIVATE, &CMyWnd::OnActivate);
//		add_handler(WM_SETFOCUS, &CMyWnd::OnSetFocus);
//		add_handler(WM_KILLFOCUS, &CMyWnd::OnKillFocus);
//	}
//
//	LRESULT OnCreate(LPARAM lparam, WPARAM wparam)
//	{
//		LogNoise(L"lparam: %Id wparam: %Id\n", lparam, wparam);
//		return 0;
//	}
//
//	LRESULT OnDestroy(LPARAM lparam, WPARAM wparam)
//	{
//		LogNoise(L"lparam: %Id wparam: %Id\n", lparam, wparam);
//		::PostQuitMessage(0);
//		return 0;
//	}
//
//	LRESULT OnClose(LPARAM lparam, WPARAM wparam)
//	{
//		// chose close option
//		LogNoise(L"lparam: %Id wparam: %Id\n", lparam, wparam);
//		::DestroyWindow(get_handle());
//		return 0;
//	}
//
//	LRESULT OnActivate(LPARAM lparam, WPARAM wparam)
//	{
//		LogNoise(L"lparam: %Id wparam: %Id\n", lparam, wparam);
//		return 0;
//	}
//
//	LRESULT OnSetFocus(LPARAM lparam, WPARAM wparam)
//	{
//		LogNoise(L"lparam: %Id wparam: %Id\n", lparam, wparam);
//		return 0;
//	}
//
//	LRESULT OnKillFocus(LPARAM lparam, WPARAM wparam)
//	{
//		LogNoise(L"lparam: %Id wparam: %Id\n", lparam, wparam);
//		return 0;
//	}
//};
//
//int test_window()
//{
//	LogTrace();
//	CMyWnd * wnd = new CMyWnd;
//	wnd->create(0, L"HelloWorld!", 0, WS_OVERLAPPEDWINDOW | WS_VISIBLE, 300, 300, 500, 400, 0);
//
//	Base::WindowApplication().Run();
//	return 0;
//}
//
namespace {
	void setup_logger()
	{
		using namespace logger;
		Default::set_level(Level::Trace);
		Default::set_prefix(Prefix::Full & ~Prefix::Date);
		Default::set_target(get_TargetToConsole());
//		Default::set_target(get_TargetToFile(L"test-threads.log", true));

//		set_module_enabled(false, get_module(L"threads"));
	}
}

struct Routine: public thread::Routine
{
	Routine(sync::Queue * queue, ssize_t num):
		m_queue(queue),
		m_num(num)
	{
		LogTrace();
	}

	size_t run(void *) override
	{
		LogDebug(L"Start routine\n");
		sync::Message message;
		m_queue->get_message(message, 5000);
		LogDebug(L"Exit routine\n");
		return m_num;
	}

private:
	sync::Queue * m_queue;
	ssize_t m_num;
};

int test_threads()
{
	sync::Queue queue;
	Routine routine1(&queue, 100);
	Routine routine2(&queue, 200);
	thread::Pool threads;
	threads.create_thread(&routine1);
	threads.create_thread(&routine2);

//	Sleep(5000);
	threads[0].set_io_priority(thread::IoPriority::VERY_LOW);
	threads[0].set_io_priority(thread::IoPriority::NORMAL);
	threads[1].set_io_priority(thread::IoPriority::LOW);
	threads[1].set_io_priority(thread::IoPriority::HIGH);
	threads[1].set_io_priority(thread::IoPriority::CRITICAL);

//	Sleep(5000);
	threads[0].set_priority(thread::Priority::TIME_CRITICAL);
	threads[1].set_priority(thread::Priority::ABOVE_NORMAL);

	sync::Message message(1, 2, 3, nullptr);
	queue.put_message(message);
//	queue.put_message(message);
	while (threads.wait_all(1000) != sync::WaitResult_t::SUCCESS)
		;

	LogInfo(L"threads[0] exited: %d\n", threads[0].get_exitcode());
	LogInfo(L"threads[1] exited: %d\n", threads[1].get_exitcode());
	return 0;
}


#ifdef NDEBUG
int wWmain()
#else
int main()
#endif
{
	setup_logger();
	LogTrace();

//	return test_window();

	return test_threads();

//	return test_sstr();
}

/// ========================================================================== Startup (entry point)
#ifdef NDEBUG
extern "C" {
	int atexit(Base::FunctionAtExit pf)
	{
		return Base::atexit(pf);
	}

	void __cxa_pure_virtual(void)
	{
		Base::cxa_pure_virtual();
	}

	int	mainCRTStartup() {
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
