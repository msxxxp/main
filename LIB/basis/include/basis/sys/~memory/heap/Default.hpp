#ifndef BASIS_SYS_MEMORY_HEAP_DEFAULT_HPP_
#define BASIS_SYS_MEMORY_HEAP_DEFAULT_HPP_

namespace memory {
	namespace heap {

		struct Default
		{
			static void   init(size_t size = 0);
			static void   destroy();
			static size_t size();

			static void*  alloc(size_t size, size_t flags = 0);
			static void*  realloc(void* ptr, size_t size, size_t flags = 0);
			static void   free(const void* ptr);
			static size_t size(const void* ptr);

			static const char* get_name();
		};

		struct DefaultMovable
		{
			static void   init(size_t size = 0);
			static void   destroy();
			static size_t size();

			static void*  alloc(size_t size, size_t flags = 0);
			static void*  realloc(void* ptr, size_t size, size_t flags = 0);
			static void   free(const void* hndl);
			static void*  lock(void* hndl);
			static void   unlock(const void* hndl);
			static size_t size(const void* ptr);

			static const char* get_name();
		};

	}
}

#endif
