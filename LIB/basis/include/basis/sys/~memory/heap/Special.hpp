#ifndef BASIS_SYS_MEMORY_HEAP_SPECIAL_HPP_
#define BASIS_SYS_MEMORY_HEAP_SPECIAL_HPP_

#include <basis/sys/memory.hpp>
#include <basis/sys/crt.hpp>
#include <basis/sys/console.hpp>

namespace memory {
	namespace heap {

		template<typename Type>
		struct Special
		{
			static void   init(size_t size = 0);
			static void   destroy();
			static void*  alloc(size_t size, size_t flags = 0);
			static void*  realloc(void* ptr, size_t size, size_t flags = 0);
			static void   free(const void* ptr);
			static size_t size(const void* ptr);
			static size_t size();
			static const Stat& get_stat();

		private:
			static HANDLE m_heap;
			static Stat   m_stat;
		};

	}
}

namespace memory {
	namespace heap {

		template<typename Type>
		HANDLE Special<Type>::m_heap;

		template<typename Type>
		Stat Special<Type>::m_stat;

		template<typename Type>
		void Special<Type>::init(size_t size)
		{
			UNUSED(size);
			m_heap = HeapCreate(0, 0, 0);
			CRT_ASSERT(m_heap);
		}

		template<typename Type>
		void Special<Type>::destroy()
		{
			CRT_ASSERT(m_heap);
			HeapDestroy(m_heap);
		}

		template<typename Type>
		void* Special<Type>::alloc(size_t size, size_t flags)
		{
			CRT_ASSERT(m_heap);
			void* ret = HeapAlloc(m_heap, flags, size);
			if (ret) {
				++m_stat.allocations;
				m_stat.allocSize += size;
			}
			return ret;
		}

		template<typename Type>
		void* Special<Type>::realloc(void* ptr, size_t size, size_t flags)
		{
			size_t freeSize = HeapSize(m_heap, 0, ptr);
			void* ret = HeapReAlloc(m_heap, flags, ptr, size);
			if (ret) {
				++m_stat.frees;
				++m_stat.allocations;
				m_stat.freeSize += freeSize;
				m_stat.allocSize += size;
			}
			return ret;
		}

		template<typename Type>
		void Special<Type>::free(const void* ptr)
		{
			CRT_ASSERT(m_heap);
			if (ptr) {
				++m_stat.frees;
				m_stat.freeSize += HeapSize(m_heap, 0, ptr);
			}
			HeapFree(m_heap, 0, const_cast<void*>(ptr));
		}

		template<typename Type>
		size_t Special<Type>::size(const void* ptr)
		{
			CRT_ASSERT(m_heap);
			return HeapSize(m_heap, 0, ptr);
		}

		template<typename Type>
		size_t Special<Type>::size()
		{
			return static_cast<size_t>(-1);
		}

		template<typename Type>
		const Stat& Special<Type>::get_stat()
		{
			return m_stat;
		}

	}
}

#endif
