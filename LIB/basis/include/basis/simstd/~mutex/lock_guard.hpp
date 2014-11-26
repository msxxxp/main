#ifndef BASIS_MUTEX_LOCK_GUARD_HPP_
#define BASIS_MUTEX_LOCK_GUARD_HPP_

#include <basis/simstd/~mutex/types.hpp>
#include <basis/ext/pattern.hpp>

namespace simstd {

	template<typename Mutex>
	class lock_guard: private pattern::Uncopyable
	{
	public:
		typedef Mutex mutex_type;

		~lock_guard();

		explicit lock_guard(mutex_type& m);

		explicit lock_guard(defer_lock_t);

		lock_guard(mutex_type& m, adopt_lock_t);

		lock_guard(lock_guard&& other);

	private:
		mutex_type* m_sync;
	};

	template<typename Mutex>
	lock_guard<Mutex>::~lock_guard()
	{
		if (m_sync)
			m_sync->unlock();
	}

	template<typename Mutex>
	lock_guard<Mutex>::lock_guard(mutex_type& m) :
		m_sync(&m)
	{
		m_sync->lock();
	}

	template<typename Mutex>
	lock_guard<Mutex>::lock_guard(defer_lock_t) :
		m_sync(nullptr)
	{
	}

	template<typename Mutex>
	lock_guard<Mutex>::lock_guard(mutex_type& m, adopt_lock_t) :
		m_sync(&m)
	{
	}

	template<typename Mutex>
	lock_guard<Mutex>::lock_guard(lock_guard&& other) :
		m_sync(other.m_sync)
	{
		other.m_sync = nullptr;
	}

	template<typename Mutex>
	lock_guard<Mutex> auto_lock(Mutex& mutex)
	{
		return simstd::move(lock_guard<Mutex>(mutex));
	}

}

#endif
