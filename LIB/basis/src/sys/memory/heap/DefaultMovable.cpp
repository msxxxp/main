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

		void* DefaultMovable::alloc(size_t size, size_t flags)
		{
			return HeapAlloc(GetProcessHeap(), flags, size);
		}

		void* DefaultMovable::realloc(void* ptr, size_t size, size_t flags)
		{
			return HeapReAlloc(GetProcessHeap(), flags, ptr, size);
		}

		void DefaultMovable::free(const void* hndl)
		{
			HeapFree(GetProcessHeap(), 0, const_cast<void*>(hndl));
		}

		void* DefaultMovable::lock(void * hndl)
		{
			return hndl;
		}

		void DefaultMovable::unlock(const void* /*hndl*/)
		{
		}

		size_t DefaultMovable::size()
		{
			return static_cast<size_t>(-1);
		}

		size_t DefaultMovable::size(const void* ptr)
		{
			return HeapSize(GetProcessHeap(), 0, ptr);
		}

	}
}
