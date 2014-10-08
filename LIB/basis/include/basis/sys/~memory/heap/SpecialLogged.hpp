#ifndef BASIS_SYS_MEMORY_HEAP_SPECIALLOGGED_HPP_
#define BASIS_SYS_MEMORY_HEAP_SPECIALLOGGED_HPP_

#include <basis/sys/memory.hpp>
#include <basis/sys/crt.hpp>
#include <basis/sys/console.hpp>

namespace memory {
	namespace heap {

		template<typename Type>
		struct SpecialLogged
		{
			static void init(size_t size = 0);
			static void destroy();
			static void* alloc(size_t size);
			static void free(const void* ptr);
			static size_t size();
			static size_t size(const void* ptr);
			static const Stat& get_stat();

		private:
			static HANDLE   m_heap;
			static Stat m_stat;
		};

	}
}

namespace memory {
	namespace heap {

		template<typename Type>
		HANDLE SpecialLogged<Type>::m_heap;

		template<typename Type>
		Stat SpecialLogged<Type>::m_stat;

		template<typename Type>
		void SpecialLogged<Type>::init(size_t size)
		{
			UNUSED(size);
			m_heap = HeapCreate(0, 0, 0);
			CRT_ASSERT(m_heap);
		}

		template<typename Type>
		void SpecialLogged<Type>::destroy()
		{
			CRT_ASSERT(m_heap);
			HeapDestroy(m_heap);
		}

		template<typename Type>
		void* SpecialLogged<Type>::alloc(size_t size)
		{
			CRT_ASSERT(m_heap);
			void* ret = HeapAlloc(m_heap, 0, size);
			console::printf(L"<%5u> %S() [%Iu] -> %p\n", GetCurrentThreadId(), __PRETTY_FUNCTION__, size, ret);
			if (ret) {
				++m_stat.allocations;
				m_stat.allocSize += size;
			}
			return ret;
		}

		template<typename Type>
		void SpecialLogged<Type>::free(const void* ptr)
		{
			CRT_ASSERT(m_heap);
			console::printf(L"<%5u> %S() [%p, %Iu]\n", GetCurrentThreadId(), __PRETTY_FUNCTION__, ptr, HeapSize(m_heap, 0, ptr));
			if (ptr) {
				++m_stat.frees;
				m_stat.freeSize += HeapSize(m_heap, 0, ptr);
			}
			HeapFree(m_heap, 0, const_cast<void*>(ptr));
		}

		template<typename Type>
		size_t SpecialLogged<Type>::size(const void* ptr)
		{
			CRT_ASSERT(m_heap);
			return HeapSize(m_heap, 0, ptr);
		}

		template<typename Type>
		size_t SpecialLogged<Type>::size()
		{
			return static_cast<size_t>(-1);
		}

		template<typename Type>
		const Stat& SpecialLogged<Type>::get_stat()
		{
			return m_stat;
		}

	}
}

#endif
