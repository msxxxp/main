#ifndef LIBSTL_MUTEX_UNIQUE_LOCK_HPP_
#define LIBSTL_MUTEX_UNIQUE_LOCK_HPP_

#include <extra/pattern.hpp>

//#include <atomic>
#include <tuple>

namespace simstd {

	/// Do not acquire ownership of the mutex.
	struct defer_lock_t {
	};

	/// Try to acquire ownership of the mutex without blocking.
	struct try_to_lock_t {
	};

	/// Assume the calling thread has already obtained mutex ownership
	/// and manage it.
	struct adopt_lock_t {
	};

	constexpr defer_lock_t defer_lock{};
	constexpr try_to_lock_t try_to_lock{};
	constexpr adopt_lock_t adopt_lock{};

	template<typename SyncUnit>
	class unique_lock: private pattern::Uncopyable {
		typedef unique_lock<SyncUnit> this_type;

	public:
		typedef SyncUnit mutex_type;

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
			this_type(std::move(other)).swap(*this);
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
			using std::swap;
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

	namespace pvt {
		template<typename SyncUnit>
		unique_lock<SyncUnit> try_to_lock(SyncUnit & l)
		{
			return unique_lock<SyncUnit>(l, Lock::try_to_lock);
		}

		template<int Index, bool Continue = true>
		struct try_lock_impl {
			template<typename ... SyncUnit>
			static int do_try_lock(std::tuple<SyncUnit & ...> & locks)
			{
				int idx = Index;
				auto lock = try_to_lock(std::get<Index>(locks));
				if (lock.owns_lock()) {
					idx = try_lock_impl<Index + 1, Index + 2 < sizeof...(SyncUnit)>::do_try_lock(locks);
					if (idx == -1)
						lock.release();
				}
				return idx;
			}
		};

		template<int Index>
		struct try_lock_impl<Index, false> {
			template<typename ... SyncUnit>
			static int do_try_lock(std::tuple<SyncUnit & ...> & locks)
			{
				int idx = Index;
				auto lock = try_to_lock(std::get<Index>(locks));
				if (lock.owns_lock()) {
					lock.release();
					idx = -1;
				}
				return idx;
			}
		};

	}

	template<typename SyncUnit1, typename SyncUnit2, typename ... SyncUnit3>
	int try_lock(SyncUnit1 & l1, SyncUnit2 & l2, SyncUnit3 & ... l3)
	{
		auto locks = std::tie(l1, l2, l3...);
		return pvt::try_lock_impl<0>::do_try_lock(locks);
	}

	template<typename SyncUnit1, typename SyncUnit2, typename ... SyncUnit3>
	void lock(SyncUnit1 & l1, SyncUnit2 & l2, SyncUnit3 & ... l3)
	{
		while (true) {
			unique_lock<SyncUnit1> first(l1);
			auto locks = std::tie(l2, l3...);
			if (pvt::try_lock_impl<0, sizeof...(SyncUnit3)>::do_try_lock(locks) == -1) {
				first.release();
				return;
			}
		}
	}

} // namespace Lock

#endif
