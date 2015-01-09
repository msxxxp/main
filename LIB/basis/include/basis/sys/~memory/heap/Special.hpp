#ifndef BASIS_SYS_MEMORY_HEAP_SPECIAL_HPP_
#define BASIS_SYS_MEMORY_HEAP_SPECIAL_HPP_

//#include <basis/sys/memory.hpp>
#include <basis/sys/crt.hpp>

namespace memory {
	namespace heap {

		template<typename Type>
		struct Special
		{
			static void   init(size_t size = 0);
			static void   destroy();
			static size_t size();

			static void*  alloc(size_t size, size_t flags = 0);
			static void*  realloc(void* ptr, size_t size, size_t flags = 0);
			static void   free(const void* ptr);
			static size_t size(const void* ptr);

		private:
			static HANDLE m_heap;
		};

	}
}

namespace memory {
	namespace heap {

		template<typename Type>
		HANDLE Special<Type>::m_heap;

		template<typename Type>
		void Special<Type>::init(size_t size)
		{
			UNUSED(size);
			CRT_ASSERT(m_heap == nullptr);
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
			return HeapAlloc(m_heap, flags, size);
		}

		template<typename Type>
		void* Special<Type>::realloc(void* ptr, size_t size, size_t flags)
		{
			CRT_ASSERT(m_heap);
			return HeapReAlloc(m_heap, flags, ptr, size);
		}

		template<typename Type>
		void Special<Type>::free(const void* ptr)
		{
			CRT_ASSERT(m_heap);
			HeapFree(m_heap, 0, const_cast<void*>(ptr));
		}

		template<typename Type>
		size_t Special<Type>::size(const void* ptr)
		{
			CRT_ASSERT(m_heap);
			return ptr ? HeapSize(m_heap, 0, ptr) : 0;
		}

		template<typename Type>
		size_t Special<Type>::size()
		{
			return static_cast<size_t>(-1);
		}

	}
}

#endif
