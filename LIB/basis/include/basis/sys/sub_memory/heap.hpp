#ifndef BASIS_SYS_MEMORY_HEAP_HPP_
#define BASIS_SYS_MEMORY_HEAP_HPP_

#include <basis/sys/memory.hpp>

namespace memory {

	struct Heap {
		~Heap();

		Heap(size_t size);

		void* alloc(size_t size);

		void free(const void* ptr);

		size_t size() const;

		size_t size(const void* ptr) const;

	private:
		HANDLE m_heap;
	};

	struct HeapWatch: public Heap {
		HeapWatch(size_t size);

		void* alloc(size_t size);

		void free(const void* ptr);

		using Heap::size;

	private:
		HANDLE m_heap;
	};

	struct HeapSpecial1
	{
		static void init(size_t size);

		static void destroy();

		static void* alloc(size_t size);

		static void free(const void* ptr);

		static size_t size();

		static size_t size(const void* ptr);

	private:
		static HANDLE m_heap;
	};

}

#endif
