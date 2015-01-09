#ifndef _MSC_VER

#include <basis/configure.hpp>
#include <basis/sys/~memory/heap/Host.hpp>

void operator delete [](void * ptr) noexcept
{
	memory::heap::DefaultHost::free(ptr);
}

#endif
