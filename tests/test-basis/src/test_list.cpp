#include <tests.hpp>
#include <A.hpp>

#include <basis/sys/console.hpp>
#include <basis/sys/logger.hpp>
#include <basis/sys/memory.hpp>

#include <basis/simstd/list>
#include <basis/simstd/algorithm>
#include <basis/simstd/memory>

//#include <algorithm>
//#include <vector>
//#include <string>

typedef memory::heap::DefaultStatTag<A> heap_type;
typedef simstd::AllocatorHeap<A, heap_type> EqAlloc;

void test_list()
{
	LogTrace();
//	std::string a;

	A ma[7];

	LogTrace();

	{
		heap_type::init();

//		LogTrace();
//		A a;
		simstd::list<A, EqAlloc> list1;
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
//
//		list1.pop_back();
//		LogInfo(L"1:size(): %Iu\n", list1.size());
//		LogInfo(L"1:empty(): %d\n", list1.empty());
//
//		for (auto it = list1.begin(); it != list1.end(); ++it)
//			LogInfo(L"%d\n", it->val());
//
//		simstd::list<A> list2(5, a);
//		LogInfo(L"2:size(): %Iu\n", list2.size());
//		LogInfo(L"2:empty(): %d\n", list2.empty());
//
//		simstd::list<A> list3(10);
//		LogInfo(L"3:size(): %Iu\n", list3.size());
//		LogInfo(L"3:empty(): %d\n", list3.empty());
//
//		simstd::list<A> list4(simstd::begin(ma), simstd::end(ma));
//		LogInfo(L"4:size(): %Iu\n", list4.size());
//		LogInfo(L"4:empty(): %d\n", list4.empty());
//
//		simstd::list<A> list4a(list4.begin(), list4.end());
//		LogInfo(L"4a:size(): %Iu\n", list4a.size());
//		LogInfo(L"4a:empty(): %d\n", list4a.empty());
//
//		simstd::list<A> list5(list2);
//		LogInfo(L"5:size(): %Iu\n", list5.size());
//		LogInfo(L"5:empty(): %d\n", list5.empty());
//
//		simstd::list<A> list6(simstd::move(list3));
//		LogInfo(L"6:size(): %Iu\n", list6.size());
//		LogInfo(L"6:empty(): %d\n", list6.empty());
//		LogInfo(L"3:size(): %Iu\n", list3.size());
//		LogInfo(L"3:empty(): %d\n", list3.empty());

//		list1.emplace_back(a);
//
//		list1.push_back(a);
//
//		list1.push_back(a + a);
//
//		list1.push_back(simstd::move(a));
	}
	{
		const memory::heap::Stat& stat = heap_type::get_stat();
		console::printf("stat alloc: %I64u, %I64u\n", stat.allocations, stat.allocSize);
		console::printf("stat free : %I64u, %I64u\n", stat.frees, stat.freeSize);
		console::printf("stat diff : %I64d\n", stat.allocSize - stat.freeSize);
	}
}
