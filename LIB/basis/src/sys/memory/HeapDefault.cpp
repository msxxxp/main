#include <basis/configure.hpp>
#include <basis/sys/memory.hpp>

namespace memory {

	void HeapDefault::init(size_t /*size*/)
	{
		m_heap = GetProcessHeap();
	}

	void HeapDefault::destroy()
	{
	}

	void* HeapDefault::alloc(size_t size)
	{
		return HeapAlloc(m_heap, 0, size);
	}

	void HeapDefault::free(const void* ptr)
	{
		HeapFree(m_heap, 0, const_cast<void*>(ptr));
	}

	size_t HeapDefault::size()
	{
		return static_cast<size_t>(-1);
	}

	size_t HeapDefault::size(const void* ptr)
	{
		return HeapSize(m_heap, 0, ptr);
	}

	HeapStat HeapDefault::get_stat()
	{
		HeapStat tmp;
		zero(tmp);
		return tmp;
	}

	HANDLE HeapDefault::m_heap;

}
