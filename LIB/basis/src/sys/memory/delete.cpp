#ifndef _MSC_VER

#include <basis/configure.hpp>
#include <basis/sys/~memory/heap/Heap.hpp>

void operator delete(void* ptr) noexcept
{
	HostFree(memory::heap::DefaultStat, ptr);
}

#endif
