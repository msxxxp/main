#ifndef BASIS_SYS_MEMORY_HEAP_HOST_HPP_
#define BASIS_SYS_MEMORY_HEAP_HOST_HPP_

#include <basis/sys/~memory/heap/Stat.hpp>
#include <basis/sys/~memory/heap/Default.hpp>

namespace memory {
	namespace heap {

		template<typename HeapType, typename StatType, typename Tag = HeapType>
		struct Host
		{
			typedef HeapType heap_type;
			typedef StatType stat_type;

			static void   init(size_t size = 0);
			static void   destroy();
			static size_t size();

			static void*  alloc(size_t size, size_t flags = 0);
			static void*  realloc(void* ptr, size_t size, size_t flags = 0);
			static void   free(const void* ptr);
			static size_t size(const void* ptr);

			static const char* get_name();
			static const stat_type& get_stat();

		private:
			static heap_type heap;
			static stat_type stat;
		};

		typedef Host<Default, Stat> DefaultHost;

		template<typename TagType>
		struct DefaultHostTag: public Host<Default, Stat, TagType>
		{
		};
	}
}

namespace memory {
	namespace heap {

		template<typename HeapType, typename StatType, typename TagType>
		typename Host<HeapType, StatType, TagType>::heap_type Host<HeapType, StatType, TagType>::heap;

		template<typename HeapType, typename StatType, typename TagType>
		typename Host<HeapType, StatType, TagType>::stat_type Host<HeapType, StatType, TagType>::stat;

		template<typename HeapType, typename StatType, typename TagType>
		void Host<HeapType, StatType, TagType>::init(size_t size)
		{
			heap.init(size);
		}

		template<typename HeapType, typename StatType, typename TagType>
		void Host<HeapType, StatType, TagType>::destroy()
		{
			heap.destroy();
		}

		template<typename HeapType, typename StatType, typename TagType>
		size_t Host<HeapType, StatType, TagType>::size()
		{
			return heap.size();
		}

		template<typename HeapType, typename StatType, typename TagType>
		void* Host<HeapType, StatType, TagType>::alloc(size_t size, size_t flags)
		{
			return stat.commit_alloc(heap.alloc(size, flags), size);
		}

		template<typename HeapType, typename StatType, typename TagType>
		void* Host<HeapType, StatType, TagType>::realloc(void* ptr, size_t size, size_t flags)
		{
			stat.commit_free(ptr, heap.size(ptr));
			return stat.commit_alloc(heap.realloc(ptr, size, flags), size);
		}

		template<typename HeapType, typename StatType, typename TagType>
		void Host<HeapType, StatType, TagType>::free(const void* ptr)
		{
			stat.commit_free(ptr, heap.size(ptr));
			heap.free(ptr);
		}

		template<typename HeapType, typename StatType, typename TagType>
		size_t Host<HeapType, StatType, TagType>::size(const void* ptr)
		{
			return heap.size(ptr);
		}

		template<typename HeapType, typename StatType, typename TagType>
		const char* Host<HeapType, StatType, TagType>::get_name()
		{
			return heap.get_name();
		}

		template<typename HeapType, typename StatType, typename TagType>
		const typename Host<HeapType, StatType, TagType>::stat_type& Host<HeapType, StatType, TagType>::get_stat()
		{
			return stat;
		}

	}
}

#endif
