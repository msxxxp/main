#ifndef LIBBASE_PVT_UNIQUE_LOCK_HPP_
#define LIBBASE_PVT_UNIQUE_LOCK_HPP_

//#include <atomic>
#include <tuple>

namespace Lock {

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

	template<typename _Mutex>
	class unique_lock {
	public:
		typedef _Mutex mutex_type;

		unique_lock() :
			m_sync(0), m_owns(false)
		{
		}

		unique_lock(mutex_type & m) :
			m_sync(&m), m_owns(false)
		{
			lock();
		}

		unique_lock(mutex_type & m, defer_lock_t) :
			m_sync(&m), m_owns(false)
		{
		}

		unique_lock(mutex_type & m, try_to_lock_t) :
			m_sync(&m), m_owns(m_sync->try_lock())
		{
		}

		unique_lock(mutex_type & m, adopt_lock_t) :
			m_sync(&m), m_owns(true)
		{
		}

		~unique_lock()
		{
			if (m_owns)
				unlock();
		}

		unique_lock(unique_lock && u) :
			m_sync(nullptr), m_owns(false)
		{
			swap(u);
		}

		unique_lock & operator =(unique_lock && u)
		{
			unique_lock(std::move(u)).swap(*this);
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
				m_sync->unlock();
				m_owns = false;
			}
		}

		void swap(unique_lock & __u)
		{
			using std::swap;
			swap(m_sync, __u.m_sync);
			swap(m_owns, __u.m_owns);
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

		unique_lock(const unique_lock &) = delete;
		unique_lock & operator =(const unique_lock &) = delete;

	private:
		mutex_type * m_sync;
		bool m_owns;
	};

	namespace pvt {
		template<typename _Lock>
		unique_lock<_Lock> try_to_lock(_Lock & l)
		{
			return unique_lock<_Lock>(l, Lock::try_to_lock);
		}

		template<int _Idx, bool _Continue = true>
		struct try_lock_impl {
			template<typename ... _Lock>
			static void do_try_lock(std::tuple<_Lock&...> & locks, int & idx)
			{
				idx = _Idx;
				auto lock = try_to_lock(std::get<_Idx>(locks));
				if (lock.owns_lock()) {
					try_lock_impl<_Idx + 1, _Idx + 2 < sizeof...(_Lock)>::do_try_lock(locks, idx);
					if (idx == -1)
						lock.release();
				}
			}
		};

		template<int _Idx>
		struct try_lock_impl<_Idx, false> {
			template<typename ... _Lock>
			static void do_try_lock(std::tuple<_Lock&...> & locks, int & idx)
			{
				idx = _Idx;
				auto lock = try_to_lock(std::get<_Idx>(locks));
				if (lock.owns_lock()) {
					idx = -1;
					lock.release();
				}
			}
		};

	}

	template<typename _Lock1, typename _Lock2, typename ... _Lock3>
	int try_lock(_Lock1 & l1, _Lock2 & l2, _Lock3 & ... l3)
	{
		int idx = 0;
		auto locks = std::tie(l1, l2, l3...);
		pvt::try_lock_impl<0>::do_try_lock(locks, idx);
		return idx;
	}

	template<typename _L1, typename _L2, typename ..._L3>
	void lock(_L1 & l1, _L2 & l2, _L3 & ... l3)
	{
		while (true) {
			unique_lock<_L1> first(l1);
			auto locks = std::tie(l2, l3...);
			int idx = 0;
			pvt::try_lock_impl<0, sizeof...(_L3)>::do_try_lock(locks, idx);
			if (idx == -1) {
				first.release();
				return;
			}
		}
	}

} // namespace Lock

#endif
