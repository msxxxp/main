#ifndef BASIS_SYS_MEMORY_HEAP_STAT_HPP_
#define BASIS_SYS_MEMORY_HEAP_STAT_HPP_

#include <basis/types.hpp>

namespace memory {
	namespace heap {

		struct Stat
		{
			uint64_t allocations;
			uint64_t frees;
			uint64_t allocSize;
			uint64_t freeSize;
		};

	}
}

#endif
