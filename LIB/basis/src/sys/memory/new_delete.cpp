#include <basis/configure.hpp>
#include <basis/sys/memory.hpp>

#ifndef _MSC_VER

void * operator new(size_t size) noexcept
{
	return memory::malloc<void*>(size, HEAP_ZERO_MEMORY);
}

void * operator new [](size_t size) noexcept
{
	return ::operator new(size);
}

void operator delete(void * in) noexcept
{
	memory::free(in);
}

void operator delete [](void * ptr) noexcept
{
	::operator delete(ptr);
}

#endif