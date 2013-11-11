#ifndef LIBBASE_LOCK_HPP_
#define LIBBASE_LOCK_HPP_

#include <libbase/std.hpp>

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

		virtual void release() = 0;
	};

	SyncUnit_i * get_CritSection();

	SyncUnit_i * get_ReadWrite();

	///================================================================================== ScopeGuard
	struct ScopeGuard: private Base::Uncopyable {
		~ScopeGuard();

		ScopeGuard();

		ScopeGuard(SyncUnit_i * unit, bool read = false);

		ScopeGuard(ScopeGuard && right);

		ScopeGuard & operator = (ScopeGuard && right);

		void swap(ScopeGuard & right);

	private:
		ScopeGuard(const ScopeGuard & right) = delete;

		ScopeGuard & operator = (const ScopeGuard & right) = delete;

		SyncUnit_i * m_unit;
	};

	///============================================================================= CriticalSection
	struct CriticalSection: private Base::Uncopyable {
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

		void release() const
		{
			::LeaveCriticalSection(&m_sync);
		}

	private:
		mutable CRITICAL_SECTION m_sync;
	};

	///=================================================================================== Semaphore
	struct Semaphore: private Base::Uncopyable {
		~Semaphore()
		{
			::CloseHandle(m_handle);
		}

		Semaphore(PCWSTR name = nullptr):
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
