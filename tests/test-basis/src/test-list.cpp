#include <tests.hpp>
#include <A.hpp>

#include <basis/sys/console.hpp>
#include <basis/sys/logger.hpp>
#include <basis/sys/memory.hpp>

#include <basis/simstd/list>
#include <basis/simstd/algorithm>
#include <basis/simstd/memory>

void test_list()
{
	LogTrace();

	A ma[7];

	LogTrace();

	struct HeapTag {};
	typedef memory::heap::DecoratorTag<memory::heap::DefaultStat, HeapTag> heap_type;
	typedef simstd::AllocatorHeap<A, heap_type> EqAlloc;

	heap_type::init();
	{

		LogTrace();
		A a;

		typedef simstd::list<A, EqAlloc> test_list_type;
		test_list_type list1;
		LogInfo(L"1:size(): %Iu\n", list1.size());
		LogInfo(L"1:empty(): %d\n", list1.empty());

		list1.emplace_back(1);
		LogInfo(L"1:size(): %Iu\n", list1.size());
		LogInfo(L"1:empty(): %d\n", list1.empty());

		list1.emplace_back(2);
		LogInfo(L"1:size(): %Iu\n", list1.size());
		LogInfo(L"1:empty(): %d\n", list1.empty());

		list1.emplace_back(3);
		LogInfo(L"1:size(): %Iu\n", list1.size());
		LogInfo(L"1:empty(): %d\n", list1.empty());

		list1.reverse();

		list1.pop_back();
		LogInfo(L"1:size(): %Iu\n", list1.size());
		LogInfo(L"1:empty(): %d\n", list1.empty());

		for (auto it = list1.begin(); it != list1.end(); ++it)
			LogInfo(L"%d\n", it->val());

		test_list_type list2(5, a);
		LogInfo(L"2:size(): %Iu\n", list2.size());
		LogInfo(L"2:empty(): %d\n", list2.empty());

		test_list_type list3(10);
		LogInfo(L"3:size(): %Iu\n", list3.size());
		LogInfo(L"3:empty(): %d\n", list3.empty());

		test_list_type list4(simstd::begin(ma), simstd::end(ma));
		LogInfo(L"4:size(): %Iu\n", list4.size());
		LogInfo(L"4:empty(): %d\n", list4.empty());

		test_list_type list4a(list4.begin(), list4.end());
		LogInfo(L"4a:size(): %Iu\n", list4a.size());
		LogInfo(L"4a:empty(): %d\n", list4a.empty());

		test_list_type list5(list2);
		LogInfo(L"5:size(): %Iu\n", list5.size());
		LogInfo(L"5:empty(): %d\n", list5.empty());

		test_list_type list6(simstd::move(list3));
		LogInfo(L"6:size(): %Iu\n", list6.size());
		LogInfo(L"6:empty(): %d\n", list6.empty());
		LogInfo(L"3:size(): %Iu\n", list3.size());
		LogInfo(L"3:empty(): %d\n", list3.empty());

		list1.emplace_back(a);

		list1.push_back(a);

		list1.push_back(a + a);

		list1.push_back(simstd::move(a));
	}
	heap_type::destroy();

	{
		const auto stat = heap_type::get_stat();
		LogReport(L"Heap '%S' statistics:\n", heap_type::get_name());
		LogReport(L"  alloc: %I64u, %I64u\n", stat.get_allocations(), stat.get_allocations_size());
		LogReport(L"  free : %I64u, %I64u\n", stat.get_frees(), stat.get_frees_size());
		LogReport(L"  diff : %I64d\n", stat.get_allocations_size() - stat.get_frees_size());
	}
}
