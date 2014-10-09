#ifndef _MSC_VER

#include <basis/configure.hpp>
#include <basis/sys/~memory/heap/DefaultStat.hpp>

void operator delete [](void * ptr) noexcept
{
	memory::heap::DefaultStat::free(ptr);
}

#endif
