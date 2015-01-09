#include <basis/configure.hpp>
#include <basis/sys/memory.hpp>

namespace memory {
	namespace heap {

		void Default::init(size_t size)
		{
			UNUSED(size);
		}

		void Default::destroy()
		{
		}

		void* Default::alloc(size_t size, size_t flags)
		{
			return HeapAlloc(GetProcessHeap(), flags, size);
		}

		void* Default::realloc(void* ptr, size_t size, size_t flags)
		{
			return HeapReAlloc(GetProcessHeap(), flags, ptr, size);
		}

		void Default::free(const void* ptr)
		{
			HeapFree(GetProcessHeap(), 0, const_cast<void*>(ptr));
		}

		size_t Default::size()
		{
			return static_cast<size_t>(-1);
		}

		size_t Default::size(const void* ptr)
		{
			return ptr ? HeapSize(GetProcessHeap(), 0, ptr) : 0;
		}

	}
}
