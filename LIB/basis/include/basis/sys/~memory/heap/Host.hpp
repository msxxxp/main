#ifndef BASIS_SYS_MEMORY_HEAP_HOST_HPP_
#define BASIS_SYS_MEMORY_HEAP_HOST_HPP_

#include <basis/sys/~memory/heap/Stat.hpp>
#include <basis/sys/~memory/heap/Default.hpp>

#define HostAlloc(HostType, Size) HostType::alloc(Size, __FUNCTION__, __LINE__)
#define HostRealloc(HostType, Ptr, Size) HostType::realloc(Ptr, Size, __FUNCTION__, __LINE__)
#define HostFree(HostType, Ptr) HostType::free(Ptr, __FUNCTION__, __LINE__)

namespace memory {
	namespace heap {

		template<typename HeapType, typename Tag>
		struct DecoratorTag: public HeapType
		{
		};

		typedef DecoratorStat<Default, StatCount> DefaultStat;

		typedef DecoratorStat<Default, StatLog> DefaultStatLogged;

	}
}

#endif
