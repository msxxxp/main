#ifndef BASIS_SYS_MEMORY_HEAP_DEFAULTSTATTAG_HPP_
#define BASIS_SYS_MEMORY_HEAP_DEFAULTSTATTAG_HPP_

#include <basis/sys/memory.hpp>

namespace memory {
	namespace heap {

		template<typename Type>
		struct DefaultStatTag
		{
			static void   init(size_t size = 0);
			static void   destroy();
			static void*  alloc(size_t size);
			static void   free(const void* ptr);
			static size_t size();
			static size_t size(const void* ptr);
			static const Stat& get_stat();

		private:
			static Stat m_stat;
		};

	}
}

namespace memory {
	namespace heap {

		template<typename Type>
		Stat DefaultStatTag<Type>::m_stat;

		template<typename Type>
		void DefaultStatTag<Type>::init(size_t size)
		{
			UNUSED(size);
		}

		template<typename Type>
		void DefaultStatTag<Type>::destroy()
		{
		}

		template<typename Type>
		void* DefaultStatTag<Type>::alloc(size_t size)
		{
			void* ret = HeapAlloc(GetProcessHeap(), 0, size);
			if (ret) {
				++m_stat.allocations;
				m_stat.allocSize += size;
			}
			return ret;
		}

		template<typename Type>
		void DefaultStatTag<Type>::free(const void* ptr)
		{
			if (ptr) {
				++m_stat.frees;
				m_stat.freeSize += HeapSize(GetProcessHeap(), 0, ptr);
			}
			HeapFree(GetProcessHeap(), 0, const_cast<void*>(ptr));
		}

		template<typename Type>
		size_t DefaultStatTag<Type>::size(const void* ptr)
		{
			return HeapSize(GetProcessHeap(), 0, ptr);
		}

		template<typename Type>
		size_t DefaultStatTag<Type>::size()
		{
			return static_cast<size_t>(-1);
		}

		template<typename Type>
		const Stat& DefaultStatTag<Type>::get_stat()
		{
			return m_stat;
		}

	}
}
#endif
