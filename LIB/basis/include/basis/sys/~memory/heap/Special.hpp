#ifndef BASIS_SYS_MEMORY_HEAP_SPECIAL_HPP_
#define BASIS_SYS_MEMORY_HEAP_SPECIAL_HPP_

#include <basis/sys/crt.hpp>

namespace memory {
	namespace heap {

		template<typename Type>
		struct Special
		{
			static void   init(size_t size = 0);
			static void   destroy();
			static size_t size();
			static void   lock();
			static void   unlock();

			static void*  alloc(size_t size, const char* function = "", int line = 0);
			static void*  realloc(void* ptr, size_t size, const char* function = "", int line = 0);
			static void   free(const void* ptr, const char* function = "", int line = 0);
			static size_t size(const void* ptr);

			static const char* get_name();

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
		size_t Special<Type>::size()
		{
			return static_cast<size_t>(-1);
		}

		template<typename Type>
		void Special<Type>::lock()
		{
			CRT_ASSERT(m_heap);
			HeapLock(m_heap);
		}

		template<typename Type>
		void Special<Type>::unlock()
		{
			CRT_ASSERT(m_heap);
			HeapUnlock(m_heap);
		}

		template<typename Type>
		void* Special<Type>::alloc(size_t size, const char* function, int line)
		{
			UNUSED(function);
			UNUSED(line);

			CRT_ASSERT(m_heap);
			return HeapAlloc(m_heap, 0, size);
		}

		template<typename Type>
		void* Special<Type>::realloc(void* ptr, size_t size, const char* function, int line)
		{
			UNUSED(function);
			UNUSED(line);

			CRT_ASSERT(m_heap);
			return HeapReAlloc(m_heap, 0, ptr, size);
		}

		template<typename Type>
		void Special<Type>::free(const void* ptr, const char* function, int line)
		{
			UNUSED(function);
			UNUSED(line);

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
		const char* Special<Type>::get_name()
		{
			return "Special";
		}

	}
}

#endif
