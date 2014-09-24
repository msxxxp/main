#include <basis/configure.hpp>
#include <basis/sys/memory.hpp>

namespace memory {
	namespace watchdog
	{

		size_t started = 0;

		size_t allocations = 0;
		size_t deletions = 0;
		uint64_t allocations_size = 0;
		uint64_t deletions_size = 0;
		pfunc on_delete = nullptr;

		void start()
		{
			started = 1;
		}

		void stop()
		{
			started = 0;
		}

		void add_allocation(uint64_t size)
		{
			if (started) {
				++allocations;
				allocations_size += size;
			}
		}

		void remove_allocation(uint64_t size)
		{
			if (started) {
				++deletions;
				deletions_size += size;
			}
		}

		size_t get_allocations()
		{
			return allocations;
		}

		uint64_t get_allocations_size()
		{
			return allocations_size;
		}

		size_t get_deletions()
		{
			return deletions;
		}

		uint64_t get_deletions_size()
		{
			return deletions_size;
		}

//		if (Watchdog::deletions_size == Watchdog::allocations_size && Watchdog::deletions == Watchdog::allocations)
//			printf("There is no leaks\n");

	};
}
