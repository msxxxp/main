#include <system/configure.hpp>
#include <system/memory.hpp>

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
