#include <basis/configure.hpp>
#include <basis/sys/console.hpp>
#include <basis/sys/memory.hpp>

namespace memory {

	void HeapDefaultLogged::init(size_t /*size*/)
	{
		m_heap = GetProcessHeap();
	}

	void HeapDefaultLogged::destroy()
	{
	}

	void* HeapDefaultLogged::alloc(size_t size)
	{
		void* ret = HeapAlloc(m_heap, 0, size);
		console::printf(L"<%5u> %S() [%Iu] -> %p\n", GetCurrentThreadId(), __PRETTY_FUNCTION__, size, ret);
		if (ret) {
			++m_stat.allocations;
			m_stat.allocSize += size;
		}
		return ret;
	}

	void HeapDefaultLogged::free(const void* ptr)
	{
		console::printf(L"<%5u> %S() [%p]\n", GetCurrentThreadId(), __PRETTY_FUNCTION__, ptr);
		if (ptr) {
			++m_stat.frees;
			m_stat.freeSize += HeapSize(m_heap, 0, ptr);
		}
		HeapFree(m_heap, 0, const_cast<void*>(ptr));
	}

	size_t HeapDefaultLogged::size()
	{
		size_t ret = static_cast<size_t>(-1);
		console::printf(L"<%5u> %S() -> %Iu\n", GetCurrentThreadId(), __PRETTY_FUNCTION__, ret);
		return ret;
	}

	size_t HeapDefaultLogged::size(const void* ptr)
	{
		size_t ret = HeapSize(m_heap, 0, ptr);
		console::printf(L"<%5u> %S() [%p] -> %Iu\n", GetCurrentThreadId(), __PRETTY_FUNCTION__, ptr, ret);
		return ret;
	}

	const HeapStat& HeapDefaultLogged::get_stat()
	{
		return m_stat;
	}

	HANDLE HeapDefaultLogged::m_heap;
	HeapStat HeapDefaultLogged::m_stat;

}
