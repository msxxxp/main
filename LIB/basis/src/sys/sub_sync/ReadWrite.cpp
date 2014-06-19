#include <basis/sys/configure.hpp>
#include <basis/sys/sync.hpp>

namespace sync {

	struct ReadWrite_impl: public SyncUnit_i, private CriticalSection {
		~ReadWrite_impl();

		ReadWrite_impl();

		void _lock() override;

		void _lock_read() override;

		bool _try_lock() override;

		bool _try_lock_read() override;

		void _unlock() override;

	private:
		HANDLE m_EventAllowWrite; 	// Writers wait on this if a reader has access
		HANDLE m_EventAllowRead;	// Readers wait on this if a writer has access
		ssize_t m_nActive;			// Number of threads currently with access (0=no threads, >0=# of readers, -1=1 writer)
		size_t m_nWaitingWriters;	// Number of writers waiting for access
		size_t m_nWaitingReaders;	// Number of readers waiting for access
		DWORD m_WriterThreadId;
	};

	ReadWrite_impl::ReadWrite_impl():
				m_EventAllowWrite(::CreateEventW(nullptr, FALSE, FALSE, nullptr)),
				m_EventAllowRead(::CreateEventW(nullptr, TRUE, FALSE, nullptr)),
				m_nActive(0),
				m_nWaitingWriters(0),
				m_nWaitingReaders(0),
				m_WriterThreadId(0)
	{
		// Initially no readers want access, no writers want access, and no threads are accessing the resource
	}

	ReadWrite_impl::~ReadWrite_impl()
	{
		::CloseHandle(m_EventAllowRead);
		::CloseHandle(m_EventAllowWrite);
	}

	void ReadWrite_impl::_lock()
	{
		CriticalSection::lock();
		// Are there any threads accessing the resource?
		bool resourceOwned = m_nActive > 0 || (m_nActive < 0 && m_WriterThreadId != ::GetCurrentThreadId());

		if (resourceOwned) {
			// This writer must wait, increment the count of waiting writers
			++m_nWaitingWriters;
		} else {
			// This writer can write, decrement the count of active writers
			--m_nActive;
			m_WriterThreadId = ::GetCurrentThreadId();
		}
		CriticalSection::unlock();

		if (resourceOwned)
			::WaitForSingleObject(m_EventAllowWrite, INFINITE);
	}

	void ReadWrite_impl::_lock_read()
	{
		CriticalSection::lock();
		// Are there writers waiting or is a writer writing?
		bool resourceWritePending = m_nWaitingWriters || m_nActive < 0;

		if (resourceWritePending) {
			// This reader must wait, increment the count of waiting readers
			++m_nWaitingReaders;
		} else {
			// This reader can read, increment the count of active readers
			++m_nActive;
		}
		CriticalSection::unlock();

		if (resourceWritePending)
			::WaitForSingleObject(m_EventAllowRead, INFINITE);
	}

	bool ReadWrite_impl::_try_lock()
	{
		CriticalSection::lock();
		bool resourceFreed = m_nActive == 0 || (m_nActive < 0 && m_WriterThreadId == ::GetCurrentThreadId());

		if (resourceFreed) {
			--m_nActive;
			m_WriterThreadId = ::GetCurrentThreadId();
		}
		CriticalSection::unlock();

		return resourceFreed;
	}

	bool ReadWrite_impl::_try_lock_read()
	{
		CriticalSection::lock();
		bool resourceWritePending = m_nWaitingWriters || m_nActive < 0;

		if (!resourceWritePending) {
			++m_nActive;
		}
		CriticalSection::unlock();

		return !resourceWritePending;
	}

	void ReadWrite_impl::_unlock()
	{
		CriticalSection::lock();
		if (m_nActive > 0) {
			--m_nActive;	// Readers have control so a reader must be done
		} else {
			++m_nActive;	// Writers have control so a writer must be done
		}

		if (m_nActive == 0) {
			m_WriterThreadId = 0;
			// No thread has access, who should wake up?
			// NOTE: It is possible that readers could never get access
			//       if there are always writers wanting to write
			if (m_nWaitingWriters > 0) {
				// Writers are waiting and they take priority over readers
				--m_nActive;
				--m_nWaitingWriters;
				::SetEvent(m_EventAllowWrite);	// NOTE: The event will release only 1 writer thread
			} else if (m_nWaitingReaders > 0) {
				// Readers are waiting and no writers are waiting
				m_nActive = m_nWaitingReaders;	// All readers will get access
				m_nWaitingReaders = 0;			// No readers will be waiting
				::SetEvent(m_EventAllowRead);	// release all readers
				::ResetEvent(m_EventAllowRead);
			}
		}
		CriticalSection::unlock();
	}

	///=============================================================================================
	SyncUnit_i * get_ReadWrite()
	{
		return new ReadWrite_impl;
	}

}
