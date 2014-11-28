#ifndef BASIS_SYS_SYNC_SRWLOCK_HPP_
#define BASIS_SYS_SYNC_SRWLOCK_HPP_

#include <basis/sys/sync.hpp>

namespace sync {

//	struct SRWlock {
//		SRWlock() {
//			::InitializeSRWLock(&m_impl);
//		}
//	private:
//		SRWLOCK m_impl;
//	};
//
//	struct SafeStack {
//		~SafeStack()
//		{
//			_aligned_free(m_impl);
//		}
//
//		SafeStack()
//		{
//			m_impl = (PSLIST_HEADER)_aligned_malloc(sizeof(SLIST_HEADER), MEMORY_ALLOCATION_ALIGNMENT);
//			InitializeSListHead(m_impl);
//		}
//	private:
//		PSLIST_HEADER m_impl;
//	};

}

#endif
