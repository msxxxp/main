#ifndef BASIS_SYS_MEMORY_HEAP_DEFAULT_HPP_
#define BASIS_SYS_MEMORY_HEAP_DEFAULT_HPP_

namespace memory {
	namespace heap {

		struct Default
		{
			static void   init(size_t size = 0);
			static void   destroy();
			static size_t size();
			static void   lock();
			static void   unlock();

			static void*  alloc(size_t size, const char* function = "", int line = 0);
			static void*  realloc(void* ptr, size_t size, const char* function = "", int line = 0);
			static void   free(const void* ptr, const char* function = "", int line = 0);
			static size_t size(const void* ptr);

			static const char* get_name();
		};

		struct DefaultMovable
		{
			static void   init(size_t size = 0);
			static void   destroy();
			static size_t size();
			static void   lock();
			static void   unlock();

			static void*  alloc(size_t size, const char* function = "", int line = 0);
			static void*  realloc(void* ptr, size_t size, const char* function = "", int line = 0);
			static void   free(const void* ptr, const char* function = "", int line = 0);
			static size_t size(const void* ptr);

			static void*  lock(void* hndl);
			static void   unlock(const void* hndl);

			static const char* get_name();
		};

	}
}

#endif
