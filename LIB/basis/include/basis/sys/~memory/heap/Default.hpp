#ifndef BASIS_SYS_MEMORY_HEAP_DEFAULT_HPP_
#define BASIS_SYS_MEMORY_HEAP_DEFAULT_HPP_

#include <basis/sys/memory.hpp>

namespace memory {
	namespace heap {

		struct Default
		{
			static void   init(size_t size = 0);
			static void   destroy();
			static void*  alloc(size_t size);
			static void   free(const void* ptr);
			static size_t size(const void* ptr);
			static size_t size();
			static Stat   get_stat();
		};

	}
}

#endif
