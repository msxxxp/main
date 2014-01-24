#include <system/configure.hpp>
#include <system/sync.hpp>

#include <liblog/logger.hpp>

#include <simstl/vector>
#include <simstl/algorithm>
#include <simstl/utility>
#include <simstl/mutex>

namespace sync {
	struct DeliveryMapping {
		DeliveryMapping(Queue * queue, Message::type_t type_mask, Message::code_t code_mask, Delivery::filter_t filter):
			m_queue(queue),
			m_type_mask(type_mask),
			m_code_mask(code_mask),
			m_filter(filter)
		{
		}

		bool operator () (const Message & message) const
		{
			if (check_mask(message) && check_filter(message))
			{
				m_queue->put_message(message);
				return true;
			}
			return false;
		}

		bool operator == (const Queue * queue) const
		{
			return m_queue == queue;
		}

	private:
		bool check_mask(const Message & message) const
		{
			return (m_type_mask & message.get_type()) && (m_code_mask & message.get_code());
		}

		bool check_filter(const Message & message) const
		{
			return !m_filter || m_filter(message);
		}

		Queue * m_queue;
		Message::type_t m_type_mask;
		Message::code_t m_code_mask;
		Delivery::filter_t m_filter;
	};

	typedef simstd::pair<Delivery::SubscribtionId, DeliveryMapping> dm_t;

	struct Delivery_impl: private simstd::vector<dm_t> {

		Delivery_impl():
			m_id_generator(0)
		{
		}

		static Delivery_impl & inst();

		Delivery::SubscribtionId Subscribe(Queue * queue, Message::type_t type_mask, Message::code_t code_mask, Delivery::filter_t filter);

		void Unsubscribe(Delivery::SubscribtionId id);

		void Unsubscribe(const Queue * queue);

		void SendRound(const Message & message) const;

	private:
		Delivery::SubscribtionId GetNextId()
		{
			return ++m_id_generator;
		}

		Delivery::SubscribtionId m_id_generator;

		mutable CriticalSection m_cs;

		static Delivery_impl m_instance;
	};

	Delivery_impl Delivery_impl::m_instance;

	Delivery_impl & Delivery_impl::inst()
	{
		return m_instance;
	}

	Delivery::SubscribtionId Delivery_impl::Subscribe(Queue * queue, Message::type_t type_mask, Message::code_t code_mask, Delivery::filter_t filter)
	{
		simstd::lock_guard<CriticalSection> guard(m_cs);
		auto id = GetNextId();
		emplace_back(id, DeliveryMapping(queue, type_mask, code_mask, filter));
		return id;
	}

	void Delivery_impl::Unsubscribe(Delivery::SubscribtionId id)
	{
		simstd::lock_guard<CriticalSection> guard(m_cs);
		auto it = simstd::find_if(begin(), end(), [&](dm_t const& item) {return item.first == id;});
		if (it != end())
			erase(it);
	}

	void Delivery_impl::Unsubscribe(const Queue * queue)
	{
		simstd::lock_guard<CriticalSection> guard(m_cs);
		for (auto it = rbegin(); it != rend(); ++it) {
			if (it->second == queue)
				erase(it.base());
		}
	}

	void Delivery_impl::SendRound(const Message & message) const
	{
		simstd::lock_guard<CriticalSection> guard(m_cs);
//		simstd::for_each(begin(), end(), [&](dm_t const& item) {
//			item.second(message);
//		});
		for (const dm_t & item : *this) {
			item.second(message);
		}
	}

}

namespace sync {
	namespace {
		logger::Module_i * get_logger_module()
		{
			auto static module = logger::get_module(L"message");
			return module;
		}
	}

	namespace Delivery {

		SubscribtionId Subscribe(Queue * queue, Message::type_t type_mask, Message::code_t code_mask, filter_t filter)
		{
			LogTrace();
			return Delivery_impl::inst().Subscribe(queue, type_mask, code_mask, filter);
		}

		void Unsubscribe(SubscribtionId id)
		{
			LogTrace();
			Delivery_impl::inst().Unsubscribe(id);
		}

		void Unsubscribe(const Queue * queue)
		{
			LogTrace();
			Delivery_impl::inst().Unsubscribe(queue);
		}

		void SendRound(const Message & message)
		{
			LogTrace();
			Delivery_impl::inst().SendRound(message);
		}

	}
}
