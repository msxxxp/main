#include <basis/sys/sync.hpp>
#include <basis/sys/logger.hpp>

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

	class SimpleMessageManager: public MessageManager, private simstd::vector<Mapping> {
	public:
		void register_observer(const Observable* subject, Observer* observer) override;

		void unregister_observer(const Observable* subject, const Observer* observer) override;

		void unregister_observable(const Observable* subject) override;

		void unregister_observer(const Observer* observer) override;

		void notify(const Observable* subject, const Message& event) const override;

	private:
		mutable CriticalSection m_cs;
	};

	void SimpleMessageManager::register_observer(const Observable* subject, Observer* observer)
	{
//		simstd::lock_guard<CriticalSection> guard(m_cs);
		auto guard(simstd::auto_lock(m_cs));
		LogNoise2(L"[%p, %p]\n", subject, observer);
		emplace(simstd::upper_bound(cbegin(), cend(), subject, MappingLess()), subject, observer);
	}

	void SimpleMessageManager::unregister_observer(const Observable* subject, const Observer* observer)
	{
		auto guard(simstd::auto_lock(m_cs));
		LogNoise2(L"[%p, %p]\n", subject, observer);
		auto range = simstd::equal_range(begin(), end(), subject, MappingLess());
		erase(remove(range.first, range.second, observer), range.second);
	}

	void SimpleMessageManager::unregister_observable(const Observable* subject)
	{
		auto guard(simstd::auto_lock(m_cs));
		LogNoise2(L"[%p]\n", subject);
		auto range = simstd::equal_range(begin(), end(), subject, MappingLess());
		erase(range.first, range.second);
	}

	void SimpleMessageManager::unregister_observer(const Observer* observer)
	{
		auto guard(simstd::auto_lock(m_cs));
		LogNoise2(L"[%p]\n", observer);
		erase(remove(begin(), end(), observer), end());
	}

	void SimpleMessageManager::notify(const Observable* subject, const Message& event) const
	{
		auto guard(simstd::auto_lock(m_cs));
		LogNoise2(L"[%p, %p]\n", subject, &event);
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
