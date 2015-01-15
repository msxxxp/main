#include <basis/sys/crt.hpp>
#include <basis/sys/console.hpp>
#include <basis/sys/memory.hpp>

void crt::check_default_heap()
{
	typedef memory::heap::DefaultStat heap_type;
	const auto stat = heap_type::get_stat();
	console::printf("Heap '%s' statistics:\n", heap_type::get_name());
	console::printf("  allocated: %I64u, %I64u\n", stat.get_allocations(), stat.get_allocations_size());
	console::printf("  freed    : %I64u, %I64u\n", stat.get_frees(), stat.get_frees_size());
	console::printf("  diff     : %I64d, %I64d\n", stat.get_allocations() - stat.get_frees(), stat.get_allocations_size() - stat.get_frees_size());
}
