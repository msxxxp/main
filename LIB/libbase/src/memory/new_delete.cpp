#include <libbase/std.hpp>

void * operator new(size_t size) noexcept
{
	return Memory::malloc<void*>(size, HEAP_ZERO_MEMORY);
}

void * operator new [](size_t size) noexcept
{
	return ::operator new(size);
}

void operator delete(void * in) noexcept
{
	Memory::free(in);
}

void operator delete [](void * ptr) noexcept
{
	::operator delete(ptr);
}
