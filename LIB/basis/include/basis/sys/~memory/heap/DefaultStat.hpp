#ifndef BASIS_SYS_MEMORY_HEAP_DEFAULTSTAT_HPP_
#define BASIS_SYS_MEMORY_HEAP_DEFAULTSTAT_HPP_

#include <basis/sys/memory.hpp>

namespace memory {
	namespace heap {

		struct DefaultStat
		{
			static void   init(size_t size = 0);
			static void   destroy();
			static void*  alloc(size_t size);
			static void   free(const void* ptr);
			static size_t size();
			static size_t size(const void* ptr);
			static const Stat& get_stat();

		private:
			static Stat m_stat;
		};

	}
}

#endif
