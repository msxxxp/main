#include <basis/configure.hpp>
#include <basis/sys/memory.hpp>

namespace memory {

	void HeapDefaultStat::init(size_t /*size*/)
	{
		m_heap = GetProcessHeap();
	}

	void HeapDefaultStat::destroy()
	{
	}

	void* HeapDefaultStat::alloc(size_t size)
	{
		void* ret = HeapAlloc(m_heap, 0, size);
		if (ret) {
			++m_stat.allocations;
			m_stat.allocSize += size;
		}
		return ret;
	}

	void HeapDefaultStat::free(const void* ptr)
	{
		if (ptr) {
			++m_stat.frees;
			m_stat.freeSize += HeapSize(m_heap, 0, ptr);
		}
		HeapFree(m_heap, 0, const_cast<void*>(ptr));
	}

	size_t HeapDefaultStat::size()
	{
		return static_cast<size_t>(-1);
	}

	size_t HeapDefaultStat::size(const void* ptr)
	{
		return HeapSize(m_heap, 0, ptr);
	}

	const HeapStat& HeapDefaultStat::get_stat()
	{
		return m_stat;
	}

	HANDLE HeapDefaultStat::m_heap;
	HeapStat HeapDefaultStat::m_stat;

}
