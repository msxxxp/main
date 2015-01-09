#include <basis/configure.hpp>
#include <basis/sys/~memory/heap/Stat.hpp>

void* memory::heap::Stat::commit_alloc(void* ptr, uint64_t size)
{
	if (ptr) {
		++allocations;
		allocSize += size;
	}

	return ptr;
}

void memory::heap::Stat::commit_free(const void* ptr, uint64_t size)
{
	if (ptr) {
		++frees;
		freeSize += size;
	}
}
