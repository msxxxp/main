#include <libbase/lock.hpp>

namespace Lock {

	struct CriticalSection_impl: public SyncUnit_i, private CriticalSection {
		~CriticalSection_impl();

		void lock() override;

		void lock_read() override;

		bool try_lock() override;

		void unlock() override;
	};

	CriticalSection_impl::~CriticalSection_impl()
	{
	}

	void CriticalSection_impl::lock()
	{
		CriticalSection::lock();
	}

	void CriticalSection_impl::lock_read()
	{
		CriticalSection::lock();
	}

	bool CriticalSection_impl::try_lock()
	{
		return CriticalSection::try_lock();
	}

	void CriticalSection_impl::unlock()
	{
		CriticalSection::unlock();
	}

	///=============================================================================================
	SyncUnit_i * get_CritSection()
	{
		return new CriticalSection_impl;
	}

}
