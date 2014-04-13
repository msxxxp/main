#include <system/configure.hpp>
#include <system/sync.hpp>

#include <liblog/logger.hpp>

#include <simstd/vector>

namespace sync {
	namespace {
		logger::Module_i * get_logger_module()
		{
			auto static module = logger::get_module(L"message");
			return module;
		}
	}

	struct Queue::Queue_impl: private CriticalSection, private Semaphore, private simstd::vector<Message>
	{ // deque would bring exceptions dependence
		void put_message(value_type const& message);

		WaitResult_t get_message(value_type & message, size_t timeout_msec);
	};

	void Queue::Queue_impl::put_message(const value_type & message)
	{
		CriticalSection::lock();
		emplace_back(message);
		CriticalSection::unlock();
		Semaphore::unlock();
	}

	WaitResult_t Queue::Queue_impl::get_message(value_type & message, size_t timeout_msec)
	{
		auto waitResult = Semaphore::try_lock_ex(timeout_msec);
		if (waitResult == WaitResult_t::SUCCESS) {
			CriticalSection::lock();
			message = front();
//			pop_front();
			erase(begin());
			CriticalSection::unlock();
		}
		return waitResult;
	}

	Queue::~Queue()
	{
		delete m_impl;
		LogTrace();
	}

	Queue::Queue():
		m_impl(new Queue_impl)
	{
		LogTrace();
	}

	Queue::Queue(Queue && right):
		m_impl(right.m_impl)
	{
		right.m_impl = nullptr;
	}

	Queue & Queue::operator = (Queue && right)
	{
		if (this != &right)
			Queue(simstd::move(right)).swap(*this);
		return *this;
	}

	void Queue::swap(Queue & right)
	{
		using simstd::swap;
		swap(m_impl, right.m_impl);
	}

	void Queue::put_message(const Message & message)
	{
		LogNoise(L"Queue: %p Message(type: %Id, code: %Id, param: %Id, data: %p)\n", m_impl, message.get_type(), message.get_code(), message.get_param(), message.get_data());
		m_impl->put_message(message);
	}

	WaitResult_t Queue::get_message(Message & message, Timeout_t timeout_msec)
	{
		LogNoise(L"Queue: %p wait: %Id\n", m_impl, timeout_msec);
		auto ret = m_impl->get_message(message, timeout_msec);
		LogAttenIf(ret != WaitResult_t::SUCCESS, L"ret: '%s'\n", totext::c_str(ret));
		LogNoiseIf(ret == WaitResult_t::SUCCESS, L"ret: '%s' Message(type: %Id, code: %Id, param: %Id, data: %p)\n", totext::c_str(ret), message.get_type(), message.get_code(), message.get_param(), message.get_data());
		return ret;
	}
}
