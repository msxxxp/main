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

		void* Default::alloc(size_t size)
		{
			return HeapAlloc(GetProcessHeap(), 0, size);
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
			return HeapSize(GetProcessHeap(), 0, ptr);
		}

		Stat Default::get_stat()
		{
			Stat tmp;
			zero(tmp);
			return tmp;
		}

	}
}
