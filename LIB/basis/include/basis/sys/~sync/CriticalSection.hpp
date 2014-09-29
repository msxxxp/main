#ifndef BASIS_SYS_SYNC_CRITICALSECTION_HPP_
#define BASIS_SYS_SYNC_CRITICALSECTION_HPP_

#include <basis/sys/sync.hpp>

namespace sync {

	class CriticalSection: private pattern::Uncopyable
	{
		typedef CRITICAL_SECTION native_impl_type;

	public:
		typedef native_impl_type * native_handle_type;

		~CriticalSection()
		{
			::DeleteCriticalSection(&m_impl);
		}

		CriticalSection()
		{
			::InitializeCriticalSection(&m_impl);
		}

		void lock()
		{
			::EnterCriticalSection(&m_impl);
		}

		bool try_lock()
		{
			return ::TryEnterCriticalSection(&m_impl);
		}

		void unlock()
		{
			::LeaveCriticalSection(&m_impl);
		}

		native_handle_type native_handle()
		{
			return &m_impl;
		}

	private:
		native_impl_type m_impl;
	};

}

#endif
