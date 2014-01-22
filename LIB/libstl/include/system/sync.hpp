#ifndef LIBSTL_SYSTEM_SYNC_HPP_
#define LIBSTL_SYSTEM_SYNC_HPP_

#include <system/configure.hpp>
#include <system/sub_sync/CriticalSection.hpp>
#include <system/sub_sync/Mutex.hpp>
#include <system/sub_sync/Semaphore.hpp>
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
