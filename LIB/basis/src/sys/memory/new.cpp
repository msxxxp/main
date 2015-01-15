#ifndef _MSC_VER

#include <basis/configure.hpp>
#include <basis/sys/~memory/heap/Host.hpp>

void * operator new(size_t size) noexcept
{
	return memory::heap::DefaultStat::alloc(size);
}

#endif
