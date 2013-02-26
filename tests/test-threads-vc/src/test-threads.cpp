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

namespace {
	void setup_logger()
	{
		using namespace Logger;
		set_default_level(Level::Trace);
		set_default_prefix(Prefix::Medium | Prefix::Place | Prefix::Module | Prefix::Thread);
		set_default_target(get_TargetToConsole());

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

Base::Backtrace * qqq[1000000];
size_t count = 0;

void PrintBacktrace()
{
	qqq[count++] = new Base::Backtrace;
}

int main()
{
	setup_logger();
//	{
//	LogTrace();
//
//	Base::Queue queue;
//	Routine routine1(&queue, 100);
//	Routine routine2(&queue, 200);
//	Base::ThreadPool threads;
//	threads.emplace_back(&routine1);
//	threads.emplace_back(&routine2);
//
////	Sleep(5000);
//	threads[0].set_io_priority(Base::Thread::IoPriority_t::LOW);
//	threads[1].set_io_priority(Base::Thread::IoPriority_t::HIGH);
//
////	Sleep(5000);
//	threads[0].set_priority(Base::Thread::Priority_t::TIME_CRITICAL);
//	threads[1].set_priority(Base::Thread::Priority_t::ABOVE_NORMAL);
//
//	Base::Message message(1, 2, 3, nullptr);
//	queue.put_message(message);
//	threads[0].resume();
//	threads[1].resume();
//	while (threads.wait_all(1000) != Base::WaitResult_t::SUCCESS)
//		;
//
//
//	LogInfo(L"threads[0] exited: %d\n", threads[0].get_exitcode());
//	LogInfo(L"threads[1] exited: %d\n", threads[1].get_exitcode());
//	LogTrace();
//	}
	Memory::Watchdog::on_delete = PrintBacktrace;
	LogInfo(L"allocations: %Iu\n", Memory::Watchdog::allocations);
	LogInfo(L"deletions: %Iu\n", Memory::Watchdog::deletions);
	LogInfo(L"allocations_size: %I64u\n", Memory::Watchdog::allocations_size);
	LogInfo(L"deletions_size: %I64u\n", Memory::Watchdog::deletions_size);
	return 0;
}
