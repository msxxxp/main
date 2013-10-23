#include <libbase/std.hpp>
//#include <libbase/console.hpp>

void * operator new(size_t size) noexcept
{
	void * ret = Memory::malloc<void*>(size, HEAP_ZERO_MEMORY);
//	Base::Console::printf(L"%S size: %Iu -> %p\n", __PRETTY_FUNCTION__, size, ret);
	return ret;
}

void * operator new [](size_t size) noexcept
{
	return ::operator new(size);
}

void operator delete(void * in) noexcept
{
//	Base::Console::printf(L"%S %p\n", __PRETTY_FUNCTION__, in);
	Memory::free(in);
}

void operator delete [](void * ptr) noexcept
{
	::operator delete(ptr);
}
