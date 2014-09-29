#include <basis/configure.hpp>
#include <basis/sys/console.hpp>
#include <basis/sys/memory.hpp>

namespace memory {

	HeapWatch::HeapWatch(size_t size):
		Heap(size)
	{
	}

	void* HeapWatch::alloc(size_t size)
	{
		return Heap::alloc(size);
	}

	void HeapWatch::free(const void* ptr)
	{
		Heap::free(ptr);
	}

}
