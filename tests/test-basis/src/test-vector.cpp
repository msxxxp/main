#include <tests.hpp>
#include <A.hpp>

#include <basis/sys/console.hpp>
#include <basis/sys/logger.hpp>
#include <basis/sys/memory.hpp>
#include <basis/simstd/memory>
#include <basis/simstd/vector>

namespace {
	typedef simstd::AllocatorHeap<memory::heap::AllocatedItem, memory::heap::Default> Allocator;
	typedef simstd::vector<memory::heap::AllocatedItem, Allocator> vector_type;
}

void test_vector()
{
	LogWarn(L"\n");

	vector_type* vec1 = new vector_type;
	LogReport(L"size: %Iu, capa: %Iu\n", vec1->size(), vec1->capacity());

	vec1->emplace_back(nullptr, 0, "", 0);
	LogReport(L"size: %Iu, capa: %Iu\n", vec1->size(), vec1->capacity());
}
