#include <basis/configure.hpp>
#include <basis/sys/console.hpp>
#include <basis/sys/memory.hpp>

namespace memory {
	namespace heap {

		Heap::~Heap()
		{
			HeapDestroy(m_heap);
		}

		Heap::Heap(size_t /*size*/)
		{
			m_heap = HeapCreate(0, 0, 0);
		}

		size_t Heap::size() const
		{
			return static_cast<size_t>(-1);
		}

		void* Heap::alloc(size_t size)
		{
			return HeapAlloc(m_heap, 0, size);
		}

		void Heap::free(const void* ptr)
		{
			HeapFree(m_heap, 0, const_cast<void*>(ptr));
		}

		size_t Heap::size(const void* ptr) const
		{
			return HeapSize(m_heap, 0, ptr);
		}

	}
}
