#ifndef BASIS_SYS_MEMORY_HEAP_HPP_
#define BASIS_SYS_MEMORY_HEAP_HPP_

#include <basis/sys/memory.hpp>

namespace memory {

	struct Heap {
		~Heap();

		Heap();

		void* alloc(size_t size);

		void free(void* ptr);

		size_t size() const;

	private:
		HANDLE m_heap;
	};

	struct HeapSpecial1
	{
		static void init();

		static void destroy();

		static void* alloc(size_t size);

		static void free(void* ptr);

		static size_t size();

	private:
		static HANDLE m_heap;
	};

}

#endif
