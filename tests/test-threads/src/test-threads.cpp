#include <libbase/std.hpp>
#include <libbase/backtrace.hpp>
#include <libbase/logger.hpp>
#include <libbase/messaging.hpp>
#include <libbase/thread.hpp>
#include <libbase/ThreadPool.hpp>

#include <stdio.h>
#include <functional>
#include <memory>
#include <vector>
#include <libbase/window.hpp>

struct CMyWnd: public Base::Window {
	CMyWnd()
	{
		LogTrace();
		add_handler(WM_CREATE, &CMyWnd::OnCreate);
		add_handler(WM_DESTROY, &CMyWnd::OnDestroy);
		add_handler(WM_CLOSE, &CMyWnd::OnClose);
		add_handler(WM_ACTIVATE, &CMyWnd::OnActivate);
		add_handler(WM_SETFOCUS, &CMyWnd::OnSetFocus);
		add_handler(WM_KILLFOCUS, &CMyWnd::OnKillFocus);
	}

	LRESULT OnCreate(LPARAM lparam, WPARAM wparam)
	{
		LogNoise(L"lparam: %Id wparam: %Id\n", lparam, wparam);
		return 0;
	}

	LRESULT OnDestroy(LPARAM lparam, WPARAM wparam)
	{
		LogNoise(L"lparam: %Id wparam: %Id\n", lparam, wparam);
		::PostQuitMessage(0);
		return 0;
	}

	LRESULT OnClose(LPARAM lparam, WPARAM wparam)
	{
		// chose close option
		LogNoise(L"lparam: %Id wparam: %Id\n", lparam, wparam);
		::DestroyWindow(get_handle());
		return 0;
	}

	LRESULT OnActivate(LPARAM lparam, WPARAM wparam)
	{
		LogNoise(L"lparam: %Id wparam: %Id\n", lparam, wparam);
		return 0;
	}

	LRESULT OnSetFocus(LPARAM lparam, WPARAM wparam)
	{
		LogNoise(L"lparam: %Id wparam: %Id\n", lparam, wparam);
		return 0;
	}

	LRESULT OnKillFocus(LPARAM lparam, WPARAM wparam)
	{
		LogNoise(L"lparam: %Id wparam: %Id\n", lparam, wparam);
		return 0;
	}
};

int test_window()
{
	LogTrace();
	CMyWnd * wnd = new CMyWnd;
	wnd->create(0, L"HelloWorld!", 0, WS_OVERLAPPEDWINDOW | WS_VISIBLE, 300, 300, 500, 400, 0);

	Base::WindowApplication().Run();
	return 0;
}

namespace {
	void setup_logger()
	{
		using namespace Logger;
		set_default_level(Level::Trace);
//		set_default_prefix(Prefix::Medium | Prefix::Place | Prefix::Module | Prefix::Thread);
//		set_default_target(get_TargetToConsole());
		set_default_prefix(Prefix::Full/* | Prefix::Place*/);
		set_default_target(get_TargetToFile(L"test-7zip.log", true));

//		set_module_enabled(false, get_module(L"threads"));
	}
}

struct Routine: public Base::ThreadRoutine_i
{
	Routine(Base::Queue * queue, ssize_t num):
		m_queue(queue),
		m_num(num)
	{
		LogTrace();
	}

	size_t run(void *) override
	{
		LogTrace();
		Base::Message message;
		m_queue->get_message(message, 5000);
		LogTrace();
		return m_num;
	}

private:
	Base::Queue * m_queue;
	ssize_t m_num;
};

int main()
{
	setup_logger();
	LogTrace();

//	test_window();
//	return 0;

	Base::Queue queue;
	Routine routine1(&queue, 100);
	Routine routine2(&queue, 200);
	Base::ThreadPool threads;
	threads.emplace_back(&routine1);
	threads.emplace_back(&routine2);

//	Sleep(5000);
	threads[0].set_io_priority(Base::Thread::IoPriority_t::LOW);
	threads[1].set_io_priority(Base::Thread::IoPriority_t::HIGH);

//	Sleep(5000);
	threads[0].set_priority(Base::Thread::Priority_t::TIME_CRITICAL);
	threads[1].set_priority(Base::Thread::Priority_t::ABOVE_NORMAL);

	Base::Message message(1, 2, 3, nullptr);
	queue.put_message(message);
	threads[0].resume();
	threads[1].resume();
	while (threads.wait_all(1000) != Base::WaitResult_t::SUCCESS)
		;

	LogInfo(L"threads[0] exited: %d\n", threads[0].get_exitcode());
	LogInfo(L"threads[1] exited: %d\n", threads[1].get_exitcode());
	LogTrace();
	return 0;
}
