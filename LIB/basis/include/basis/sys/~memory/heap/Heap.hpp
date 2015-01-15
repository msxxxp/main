#ifndef BASIS_SYS_MEMORY_HEAP_HEAP_HPP_
#define BASIS_SYS_MEMORY_HEAP_HEAP_HPP_

namespace memory {
	namespace heap {

		struct Heap {
			~Heap();
			Heap(size_t size = 0);
			size_t size() const;

			void*  alloc(size_t size);
			void   free(const void* ptr);
			size_t size(const void* ptr) const;

		private:
			HANDLE m_heap;
		};

	}
}

#endif
