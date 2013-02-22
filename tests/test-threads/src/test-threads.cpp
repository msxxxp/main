#include <libbase/std.hpp>
#include <libbase/logger.hpp>
#include <libbase/messaging.hpp>
#include <libbase/thread.hpp>

#include <stdio.h>
#include <functional>
#include <memory>
#include <vector>

namespace {
	void setup_logger()
	{
		using namespace Base::Logger;
		set_default_level(Level::Trace);
		set_default_prefix(Prefix::Medium | Prefix::Place | Prefix::Module | Prefix::Thread);
		set_default_target(get_TargetToConsole());

//		set_module_enabled(false, get_module(L"threads"));
	}
}

struct Routine: public Base::ThreadRoutine_i
{
	Routine(ssize_t num):
		m_num(num)
	{
		LogTrace();
	}

	size_t run(void *) override
	{
		LogTrace();
		Base::Message message;
		m_queue.get_message(message, 5000);
		return m_num;
	}

private:
	static Base::Queue m_queue;

	ssize_t m_num;
};

Base::Queue Routine::m_queue;

int main()
{
	setup_logger();
	LogTrace();

	Routine routine1(100);
	Routine routine2(200);
	std::vector<Base::Thread> threads;
	threads.emplace_back(Base::Thread(&routine1));
	threads.emplace_back(Base::Thread(&routine2));

//	Sleep(5000);
	threads[0].set_io_priority(Base::Thread::IoPriority_t::LOW);
	threads[1].set_io_priority(Base::Thread::IoPriority_t::HIGH);

//	Sleep(5000);
	threads[0].set_priority(Base::Thread::Priority_t::TIME_CRITICAL);
	threads[1].set_priority(Base::Thread::Priority_t::ABOVE_NORMAL);

	threads[0].resume();
	threads[1].resume();
	threads[0].wait();
	threads[1].wait();

	LogInfo(L"threads[0] exited: %d\n", threads[0].get_exitcode());
	LogInfo(L"threads[1] exited: %d\n", threads[1].get_exitcode());
	return 0;
}
