#ifndef LIBBASE_LOCK_HPP_
#define LIBBASE_LOCK_HPP_

#include <libbase/std.hpp>
#include <libbase/pvt/lock/unique_lock.hpp>
#include <libbase/pvt/lock/lock.hpp>

namespace Lock {

	struct ScopeGuard;

	///=============================================================================================
	struct SyncUnit_i: public Base::Destroyable {
		ScopeGuard lock_scope();

		ScopeGuard lock_scope_read();

		void destroy() const override;

	public:
		virtual ~SyncUnit_i();

		virtual void lock() = 0;

		virtual void lock_read() = 0;

		virtual void unlock() = 0;
	};

	SyncUnit_i * get_CritSection();

	SyncUnit_i * get_ReadWrite();

	///================================================================================== ScopeGuard
	struct ScopeGuard {
		~ScopeGuard();

		ScopeGuard();

		ScopeGuard(SyncUnit_i * unit, bool read = false);

		ScopeGuard(ScopeGuard && right);

		ScopeGuard & operator =(ScopeGuard && right);

		void swap(ScopeGuard & right);

	private:
		ScopeGuard(const ScopeGuard & right) = delete;

		ScopeGuard & operator =(const ScopeGuard & right) = delete;

		SyncUnit_i * m_unit;
	};

	///=================================================================================== LockGuard
	template<typename Mutex>
	class LockGuard {
	public:
		typedef Mutex mutex_type;

		explicit LockGuard(mutex_type & m) :
			m_sync(m)
		{
			m_sync.lock();
		}

		LockGuard(mutex_type & m, bool doNotLock) :
			m_sync(m)
		{
			UNUSED(doNotLock);
		}

		~LockGuard()
		{
			m_sync.unlock();
		}

		LockGuard(const LockGuard &) = delete;
		LockGuard& operator =(const LockGuard &) = delete;

	private:
		mutex_type & m_sync;
	};

	///============================================================================= CriticalSection
	struct CriticalSection {
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

		CriticalSection(const CriticalSection &) = delete;
		CriticalSection& operator =(const CriticalSection &) = delete;

	private:
		mutable CRITICAL_SECTION m_sync;
	};

	///=================================================================================== Semaphore
	struct Semaphore {
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

		Base::WaitResult_t wait(Base::Timeout_t wait_millisec = Base::WAIT_FOREVER) const
		{
			return (Base::WaitResult_t)::WaitForSingleObjectEx(m_handle, wait_millisec, true);
		}

		void release(size_t cnt = 1) const
		{
			::ReleaseSemaphore(m_handle, cnt, nullptr);
		}

		Semaphore(const Semaphore &) = delete;
		Semaphore& operator =(const Semaphore &) = delete;

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
