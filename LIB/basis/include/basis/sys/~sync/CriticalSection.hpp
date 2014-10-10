#ifndef BASIS_SYS_SYNC_CRITICALSECTION_HPP_
#define BASIS_SYS_SYNC_CRITICALSECTION_HPP_

#include <basis/sys/sync.hpp>
#include <basis/sys/console.hpp>

namespace sync {

	class CriticalSection: private pattern::Uncopyable
	{
		typedef CRITICAL_SECTION native_impl_type;

	public:
		typedef native_impl_type * native_handle_type;

		~CriticalSection()
		{
//			console::printf(L"%S:%d\n", __PRETTY_FUNCTION__, __LINE__);
			::DeleteCriticalSection(&m_impl);
		}

		CriticalSection()
		{
//			console::printf(L"%S:%d\n", __PRETTY_FUNCTION__, __LINE__);
			::InitializeCriticalSection(&m_impl);
		}

		void lock()
		{
//			console::printf(L"%S:%d\n", __PRETTY_FUNCTION__, __LINE__);
			::EnterCriticalSection(&m_impl);
		}

		bool try_lock()
		{
//			console::printf(L"%S:%d\n", __PRETTY_FUNCTION__, __LINE__);
			return ::TryEnterCriticalSection(&m_impl);
		}

		void unlock()
		{
//			console::printf(L"%S:%d\n", __PRETTY_FUNCTION__, __LINE__);
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
