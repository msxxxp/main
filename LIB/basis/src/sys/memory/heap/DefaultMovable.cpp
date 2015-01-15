#include <basis/configure.hpp>
#include <basis/sys/memory.hpp>

namespace memory {
	namespace heap {

		void DefaultMovable::init(size_t size)
		{
			UNUSED(size);
		}

		void DefaultMovable::destroy()
		{
		}

		size_t DefaultMovable::size()
		{
			return static_cast<size_t>(-1);
		}

		void* DefaultMovable::alloc(size_t size, const char* function, int line)
		{
			UNUSED(function);
			UNUSED(line);
			return HeapAlloc(GetProcessHeap(), 0, size);
		}

		void* DefaultMovable::realloc(void* ptr, size_t size, const char* function, int line)
		{
			UNUSED(function);
			UNUSED(line);
			return HeapReAlloc(GetProcessHeap(), 0, ptr, size);
		}

		void DefaultMovable::free(const void* hndl, const char* function, int line)
		{
			UNUSED(function);
			UNUSED(line);
			HeapFree(GetProcessHeap(), 0, const_cast<void*>(hndl));
		}

		size_t DefaultMovable::size(const void* ptr)
		{
			return HeapSize(GetProcessHeap(), 0, ptr);
		}

		void* DefaultMovable::lock(void * hndl)
		{
			return hndl;
		}

		void DefaultMovable::unlock(const void* /*hndl*/)
		{
		}

	}
}
