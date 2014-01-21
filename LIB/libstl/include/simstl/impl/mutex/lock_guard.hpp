#ifndef LIBSTL_MUTEX_LOCK_GUARD_HPP_
#define LIBSTL_MUTEX_LOCK_GUARD_HPP_

#include <simstl/impl/mutex/types.hpp>
#include <extra/pattern.hpp>

namespace simstd {

	template<typename Mutex>
	class lock_guard: private pattern::Uncopyable
	{
	public:
		typedef Mutex mutex_type;

		~lock_guard()
		{
			m_sync.unlock();
		}

		explicit lock_guard(mutex_type & m) :
			m_sync(m)
		{
			m_sync.lock();
		}

		lock_guard(mutex_type & m, adopt_lock_t) :
			m_sync(m)
		{
		}

	private:
		mutex_type & m_sync;
	};

}    // namespace simstd

#endif
