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

		~lock_guard()
		{
			if (m_sync)
				m_sync->unlock();
		}

		explicit lock_guard(mutex_type & m) :
			m_sync(&m)
		{
			m_sync->lock();
		}

		lock_guard(mutex_type & m, adopt_lock_t) :
			m_sync(&m)
		{
		}

		lock_guard(lock_guard && other) :
			m_sync(other.m_sync)
		{
			other.m_sync = nullptr;
		}

	private:
		mutex_type * m_sync;
	};

	template<typename Mutex>
	lock_guard<Mutex> auto_lock(Mutex & mutex)
	{
		return simstd::move(lock_guard<Mutex>(mutex));
	}

}    // namespace simstd

#endif
