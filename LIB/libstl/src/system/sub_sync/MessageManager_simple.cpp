#include <system/configure.hpp>
#include <system/sync.hpp>

#include "MessageManager.hpp"

#include <simstd/algorithm>
#include <simstd/vector>
#include <simstd/utility>
#include <simstd/mutex>

namespace sync {

	typedef simstd::pair<Observable *, Observer *> mapping;

	inline bool operator ==(const mapping & left, const Observer * right)
	{
		return left.second == right;
	}

	inline bool operator <(const mapping & left, const Observable * right)
	{
		return left.first < right;
	}

	inline bool operator <(const Observable * left, const mapping & right)
	{
		return left < right.first;
	}

	struct SimpleMessageManager: public MessageManager, private simstd::vector<mapping> {
		~SimpleMessageManager();

		void register_observer(Observable * subject, Observer * observer) override;

		void unregister_observer(Observable * subject, Observer * observer) override;

		void unregister_all(Observable * subject) override;

		void unregister_all(Observer * observer) override;

		void notify(const Observable * subject, Message const& event) const override;

	private:
		mutable CriticalSection m_cs;
	};

	SimpleMessageManager::~SimpleMessageManager()
	{
	}

	void SimpleMessageManager::register_observer(Observable * subject, Observer * observer)
	{
		simstd::lock_guard<CriticalSection> guard(m_cs);
		emplace(simstd::upper_bound(cbegin(), cend(), subject), subject, observer);
	}

	void SimpleMessageManager::unregister_observer(Observable * subject, Observer * observer)
	{
		simstd::lock_guard<CriticalSection> guard(m_cs);
		auto range = simstd::equal_range(begin(), end(), subject);
		erase(remove(range.first, range.second, observer), range.second);
	}

	void SimpleMessageManager::unregister_all(Observable * subject)
	{
		simstd::lock_guard<CriticalSection> guard(m_cs);
		auto range = simstd::equal_range(begin(), end(), subject);
		erase(range.first, range.second);
	}

	void SimpleMessageManager::unregister_all(Observer * observer)
	{
		simstd::lock_guard<CriticalSection> guard(m_cs);
		erase(remove(begin(), end(), observer), end());
	}

	void SimpleMessageManager::notify(const Observable * subject, const Message & event) const
	{
		simstd::lock_guard<CriticalSection> guard(m_cs);
		auto range = simstd::equal_range(begin(), end(), subject);
		simstd::for_each(range.first, range.second, [event](const mapping & pair) {
			pair.second->notify(event);
		});
	}

	MessageManager * get_simple_message_manager()
	{
		static SimpleMessageManager ret;
		return &ret;
	}

}
