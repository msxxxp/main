#ifndef LIBSTL_MUTEX_UNIQUE_LOCK_HPP_
#define LIBSTL_MUTEX_UNIQUE_LOCK_HPP_

#include <simstl/mutex>
#include <extra/pattern.hpp>

namespace simstd {

	template<typename Mutex>
	class unique_lock: private pattern::Uncopyable
	{
		typedef unique_lock<Mutex> this_type;

	public:
		typedef Mutex mutex_type;

		~unique_lock()
		{
			if (m_owns)
				unlock();
		}

		unique_lock() :
			m_sync(nullptr),
			m_owns(false)
		{
		}

		unique_lock(mutex_type & m) :
			m_sync(&m),
			m_owns(false)
		{
			lock();
		}

		unique_lock(mutex_type * m) :
			m_sync(m),
			m_owns(false)
		{
			lock();
		}

		unique_lock(mutex_type & m, defer_lock_t) :
			m_sync(&m),
			m_owns(false)
		{
		}

		unique_lock(mutex_type * m, defer_lock_t) :
			m_sync(m),
			m_owns(false)
		{
		}

		unique_lock(mutex_type & m, try_to_lock_t) :
			m_sync(&m),
			m_owns(m_sync->try_lock())
		{
		}

		unique_lock(mutex_type * m, try_to_lock_t) :
			m_sync(*m),
			m_owns(m_sync->try_lock())
		{
		}

		unique_lock(mutex_type & m, adopt_lock_t) :
			m_sync(&m),
			m_owns(true)
		{
		}

		unique_lock(mutex_type * m, adopt_lock_t) :
			m_sync(m),
			m_owns(true)
		{
		}

		unique_lock(this_type && other) :
			m_sync(nullptr),
			m_owns(false)
		{
			swap(other);
		}

		this_type & operator =(this_type && other)
		{
			this_type(simstd::move(other)).swap(*this);
			return *this;
		}

		void lock()
		{
			if (m_sync && !m_owns) {
				m_sync->lock();
				m_owns = true;
			}
		}

		bool try_lock()
		{
			if (m_sync && !m_owns) {
				m_owns = m_sync->try_lock();
			}
			return m_owns;
		}

		void unlock()
		{
			if (m_sync && m_owns) {
				m_owns = false;
				m_sync->unlock();
			}
		}

		void swap(this_type & other)
		{
			using simstd::swap;
			swap(m_sync, other.m_sync);
			swap(m_owns, other.m_owns);
		}

		mutex_type * release()
		{
			mutex_type * ret = m_sync;
			m_sync = nullptr;
			m_owns = false;
			return ret;
		}

		bool owns_lock() const
		{
			return m_owns;
		}

		explicit operator bool() const
		{
			return owns_lock();
		}

		mutex_type * mutex() const
		{
			return m_sync;
		}

	private:
		mutex_type * m_sync;
		bool m_owns;
	};

	template<typename _Mutex>
	void swap(unique_lock<_Mutex>& a, unique_lock<_Mutex>& b) noexcept
	{
		a.swap(b);
	}

}    // namespace simstd

#endif
