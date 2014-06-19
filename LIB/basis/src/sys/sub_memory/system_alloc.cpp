#include <stdint.h>
#include <stdlib.h>

extern "C" {
	void* _system_malloc(size_t size)
	{
		return malloc(size);
	}

	void _system_free(void * ptr)
	{
		free(ptr);
	}

	void* _system_movable_malloc(size_t size)
	{
		return malloc(size);
	}

	void _system_movable_free(void * handle)
	{
		free(handle);
	}

	void* _system_movable_lock(void * handle)
	{
		return handle;
	}

	void _system_movable_unlock(void * /*handle*/)
	{
	}
}
