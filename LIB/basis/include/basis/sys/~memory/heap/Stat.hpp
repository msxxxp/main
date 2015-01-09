#ifndef BASIS_SYS_MEMORY_HEAP_STAT_HPP_
#define BASIS_SYS_MEMORY_HEAP_STAT_HPP_

namespace memory {
	namespace heap {

		struct StatStub
		{
			void* commit_alloc(void* ptr, uint64_t size) {UNUSED(size); return ptr;}
			void commit_free(const void* ptr, uint64_t size) {UNUSED(size); UNUSED(ptr);}
			uint64_t get_allocations() const {return uint64_t();}
			uint64_t get_frees() const {return uint64_t();}
			uint64_t get_allocations_size() const {return uint64_t();}
			uint64_t get_frees_size() const {return uint64_t();}
		};

		struct Stat
		{
			void* commit_alloc(void* ptr, uint64_t size);
			void commit_free(const void* ptr, uint64_t size);
			uint64_t get_allocations() const {return allocations;}
			uint64_t get_frees() const {return frees;}
			uint64_t get_allocations_size() const {return allocSize;}
			uint64_t get_frees_size() const {return freeSize;}

		private:
			uint64_t allocations;
			uint64_t frees;
			uint64_t allocSize;
			uint64_t freeSize;
		};

	}
}

#endif
