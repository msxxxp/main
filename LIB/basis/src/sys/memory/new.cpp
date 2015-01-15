#ifndef _MSC_VER

#include <basis/configure.hpp>
#include <basis/sys/~memory/heap/Heap.hpp>

void * operator new(size_t size) noexcept
{
	return HostAlloc(memory::heap::DefaultStat, size);
}

#endif
