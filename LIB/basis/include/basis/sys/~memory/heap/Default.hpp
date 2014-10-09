#ifndef BASIS_SYS_MEMORY_HEAP_DEFAULT_HPP_
#define BASIS_SYS_MEMORY_HEAP_DEFAULT_HPP_

#include <basis/sys/~memory/heap/Stat.hpp>

namespace memory {
	namespace heap {

		struct Default
		{
			static void   init(size_t size = 0);
			static void   destroy();
			static void*  alloc(size_t size, size_t flags = 0);
			static void*  realloc(void* ptr, size_t size, size_t flags = 0);
			static void   free(const void* ptr);
			static size_t size(const void* ptr);
			static size_t size();
			static const Stat& get_stat();

		private:
			static Stat m_stat;
		};

	}
}

#endif
