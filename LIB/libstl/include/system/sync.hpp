#ifndef LIBSTL_SYSTEM_SYNC_HPP_
#define LIBSTL_SYSTEM_SYNC_HPP_

#include <system/configure.hpp>
#include <extra/pattern.hpp>

namespace sync {

	typedef ssize_t Timeout_t;

	const Timeout_t WAIT_FOREVER = INFINITE;

	enum class WaitResult_t : size_t {
		SUCCESS   = WAIT_OBJECT_0,
		APC       = WAIT_IO_COMPLETION,
		TIMEOUT   = WAIT_TIMEOUT,
		FAILED    = WAIT_FAILED,
		ABANDONED = WAIT_ABANDONED,
	};

	struct ScopeGuard;

	///=============================================================================================
	struct SyncUnit_i: public pattern::Destroyable, private pattern::Uncopyable {
		ScopeGuard lock_scope();

		ScopeGuard lock_scope_read();

		void destroy() const override;

		void lock(bool readOnly = false) {readOnly ? _lock_read() : _lock();}

		bool try_lock(bool readOnly = false) {return readOnly ? _try_lock_read() : _try_lock();}

		void unlock() {_unlock();}

	protected:
		SyncUnit_i() = default;
		virtual ~SyncUnit_i() = default;

	private:
		virtual void _lock() = 0;

		virtual void _lock_read() = 0;

		virtual bool _try_lock() = 0;

		virtual bool _try_lock_read() = 0;

		virtual void _unlock() = 0;
	};

	SyncUnit_i * get_CritSection();

	SyncUnit_i * get_ReadWrite();

	///================================================================================== ScopeGuard
	struct ScopeGuard: private pattern::Uncopyable {
		~ScopeGuard();

		ScopeGuard();

		ScopeGuard(SyncUnit_i * unit, bool readOnly = false);

		ScopeGuard(ScopeGuard && right);

		ScopeGuard & operator =(ScopeGuard && right);

		void swap(ScopeGuard & right);

	private:
		SyncUnit_i * m_unit;
	};

	///============================================================================= CriticalSection
	struct CriticalSection: private pattern::Uncopyable {
		~CriticalSection()
		{
			::DeleteCriticalSection(&m_sync);
		}

		CriticalSection()
		{
			::InitializeCriticalSection(&m_sync);
		}

		void lock() const
		{
			::EnterCriticalSection(&m_sync);
		}

		bool try_lock() const
		{
			return ::TryEnterCriticalSection(&m_sync);
		}

		void unlock() const
		{
			::LeaveCriticalSection(&m_sync);
		}

	private:
		mutable CRITICAL_SECTION m_sync;
	};

	///=================================================================================== Semaphore
	struct Semaphore: private pattern::Uncopyable {
		~Semaphore()
		{
			::CloseHandle(m_handle);
		}

		Semaphore(PCWSTR name = nullptr) :
			m_handle(::CreateSemaphoreW(nullptr, 0, LONG_MAX, name))
		{
		}

		operator HANDLE() const
		{
			return m_handle;
		}

		HANDLE handle() const
		{
			return m_handle;
		}

		WaitResult_t wait(Timeout_t wait_millisec = WAIT_FOREVER) const
		{
			return (WaitResult_t)::WaitForSingleObjectEx(m_handle, wait_millisec, true);
		}

		void release(size_t cnt = 1) const
		{
			::ReleaseSemaphore(m_handle, cnt, nullptr);
		}

	private:
		mutable HANDLE m_handle;
	};

///=============================================================================================
//		struct SRWlock {
//			SRWlock() {
//				::InitializeSRWLock(&m_impl);
//			}
//		private:
//			SRWLOCK m_impl;
//		};

//		///=========================================================================================
//		struct SafeStack {
//			~SafeStack()
//			{
//				_aligned_free(m_impl);
//			}
//
//			SafeStack()
//			{
//				m_impl = (PSLIST_HEADER)_aligned_malloc(sizeof(SLIST_HEADER), MEMORY_ALLOCATION_ALIGNMENT);
//				InitializeSListHead(m_impl);
//			}
//		private:
//			PSLIST_HEADER m_impl;
//		};
//

}

#endif
