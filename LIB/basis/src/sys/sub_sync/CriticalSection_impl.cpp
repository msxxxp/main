#include <basis/sys/configure.hpp>
#include <basis/sys/sync.hpp>

namespace sync {

	struct CriticalSection_impl: public SyncUnit_i, private CriticalSection {
		~CriticalSection_impl() = default;

		void _lock() override;

		void _lock_read() override;

		bool _try_lock() override;

		bool _try_lock_read() override;

		void _unlock() override;
	};

	void CriticalSection_impl::_lock()
	{
		CriticalSection::lock();
	}

	void CriticalSection_impl::_lock_read()
	{
		CriticalSection::lock();
	}

	bool CriticalSection_impl::_try_lock()
	{
		return CriticalSection::try_lock();
	}

	bool CriticalSection_impl::_try_lock_read()
	{
		return CriticalSection::try_lock();
	}

	void CriticalSection_impl::_unlock()
	{
		CriticalSection::unlock();
	}

	///=============================================================================================
	SyncUnit_i * get_CritSection()
	{
		return new CriticalSection_impl;
	}

}
