#include <basis/configure.hpp>
#include <basis/sys/sync.hpp>

#include <basis/simstd/algorithm>
#include <basis/simstd/vector>
#include <basis/simstd/utility>
#include <basis/simstd/mutex>

namespace sync {

	typedef simstd::pair<const Observable*, Observer*> Mapping;

	inline bool operator ==(const Mapping& left, const Observer* right)
	{
		return left.second == right;
	}

	struct MappingLess {
		bool operator ()(const Mapping& left, const Observable* right)
		{
			return left.first < right;
		}

		bool operator ()(const Observable* left, const Mapping& right)
		{
			return left < right.first;
		}
	};

	struct SimpleMessageManager: public MessageManager, private simstd::vector<Mapping> {
		void register_observer(const Observable* subject, Observer* observer) override;

		void unregister_observer(const Observable* subject, const Observer* observer) override;

		void unregister_all(const Observable* subject) override;

		void unregister_all(const Observer* observer) override;

		void notify(const Observable* subject, Message const& event) const override;

	private:
		mutable CriticalSection m_cs;
	};

	void SimpleMessageManager::register_observer(const Observable* subject, Observer* observer)
	{
		simstd::lock_guard<CriticalSection> guard(m_cs);
		emplace(simstd::upper_bound(cbegin(), cend(), subject, MappingLess()), subject, observer);
	}

	void SimpleMessageManager::unregister_observer(const Observable* subject, const Observer* observer)
	{
		simstd::lock_guard<CriticalSection> guard(m_cs);
		auto range = simstd::equal_range(begin(), end(), subject, MappingLess());
		erase(remove(range.first, range.second, observer), range.second);
	}

	void SimpleMessageManager::unregister_all(const Observable* subject)
	{
		simstd::lock_guard<CriticalSection> guard(m_cs);
		auto range = simstd::equal_range(begin(), end(), subject, MappingLess());
		erase(range.first, range.second);
	}

	void SimpleMessageManager::unregister_all(const Observer* observer)
	{
		simstd::lock_guard<CriticalSection> guard(m_cs);
		erase(remove(begin(), end(), observer), end());
	}

	void SimpleMessageManager::notify(const Observable* subject, const Message& event) const
	{
		simstd::lock_guard<CriticalSection> guard(m_cs);
		auto range = simstd::equal_range(begin(), end(), subject, MappingLess());
		simstd::for_each(range.first, range.second, [&event](const Mapping& pair) {
			pair.second->notify(event);
		});
	}

	MessageManager* MessageManager::get_default()
	{
		static SimpleMessageManager ret;
		return &ret;
	}

}
