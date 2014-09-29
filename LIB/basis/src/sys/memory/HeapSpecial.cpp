#include <basis/configure.hpp>
#include <basis/sys/memory.hpp>

namespace memory {

	void HeapSpecial1::init(size_t /*size*/)
	{
		m_heap = HeapCreate(0, 0, 0);
	}

	void HeapSpecial1::destroy()
	{
		HeapDestroy(m_heap);
	}

	void* HeapSpecial1::alloc(size_t size)
	{
		return HeapAlloc(m_heap, 0, size);
	}

	void HeapSpecial1::free(const void* ptr)
	{
		HeapFree(m_heap, 0, const_cast<void*>(ptr));
	}

	size_t HeapSpecial1::size()
	{
		return static_cast<size_t>(-1);
	}

	size_t HeapSpecial1::size(const void* ptr)
	{
		return HeapSize(m_heap, 0, ptr);
	}

	HANDLE HeapSpecial1::m_heap = nullptr;

}
