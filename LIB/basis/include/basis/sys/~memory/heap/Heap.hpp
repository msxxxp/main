#ifndef BASIS_SYS_MEMORY_HEAP_HEAP_HPP_
#define BASIS_SYS_MEMORY_HEAP_HEAP_HPP_

#include <basis/sys/memory.hpp>
#include <basis/sys/crt.hpp>
#include <basis/sys/console.hpp>

namespace memory {
	namespace heap {

		struct Heap {
			~Heap();
			Heap(size_t size = 0);
			void*  alloc(size_t size);
			void   free(const void* ptr);
			size_t size() const;
			size_t size(const void* ptr) const;
			Stat   get_stat();
		private:
			HANDLE m_heap;
		};

	}
}

#endif
