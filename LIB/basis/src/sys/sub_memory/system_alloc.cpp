#include <basis/sys/memory.hpp>

#include <stdint.h>
#include <stdlib.h>

extern "C" void* _system_malloc(size_t size)
{
//	return malloc(size);
	return memory::malloc<void*>(size);
}

extern "C" void _system_free(void * ptr)
{
//	free(ptr);
	return memory::free(ptr);
}

extern "C" void* _system_movable_malloc(size_t size)
{
//	return malloc(size);
	return memory::malloc<void*>(size);
}

extern "C" void _system_movable_free(void * handle)
{
//	free(handle);
	return memory::free(handle);
}

extern "C" void* _system_movable_lock(void * handle)
{
	return handle;
}

extern "C" void _system_movable_unlock(void * /*handle*/)
{
}
