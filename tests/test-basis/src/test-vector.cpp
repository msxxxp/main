#include <tests.hpp>
#include <A.hpp>

#include <basis/sys/console.hpp>
#include <basis/sys/logger.hpp>
#include <basis/sys/memory.hpp>
#include <basis/simstd/memory>
#include <basis/simstd/vector>

namespace {
	typedef memory::heap::DefaultStat heap_type;
	typedef simstd::AllocatorHeap<memory::heap::AllocatedItem, heap_type> Allocator;
	typedef simstd::vector<memory::heap::AllocatedItem, Allocator> vector_type;
}

void test_vector()
{
	LogWarn(L"\n");

	vector_type* vec1 = memory::malloc<vector_type*>(sizeof(vector_type));
	new (vec1, simstd::nothrow) vector_type();

//	vector_type* vec1 = new vector_type;
	LogReport(L"size: %Iu, capa: %Iu\n", vec1->size(), vec1->capacity());

	vec1->emplace_back(nullptr, 0, "", 0);
	LogReport(L"size: %Iu, capa: %Iu\n", vec1->size(), vec1->capacity());
//	delete vec1;
	vec1->~vector();
	memory::free(vec1);

	{
		const auto stat = heap_type::get_stat();
		LogReport(L"stat alloc: %I64u, %I64u\n", stat.get_allocations(), stat.get_allocations_size());
		LogReport(L"stat free : %I64u, %I64u\n", stat.get_frees(), stat.get_frees_size());
		LogReport(L"stat diff : %I64d\n", stat.get_allocations_size() - stat.get_frees_size());
	}
}
