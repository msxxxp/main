#include <basis/sys/logger.hpp>
#include <basis/sys/memory.hpp>

//#include <algorithm>
//#include <vector>
#include <basis/simstd/list>
#include <basis/simstd/algorithm>
#include <basis/simstd/memory>

namespace {
	void setup_logger()
	{
		LogSetOptions(L"logger:///default?level=tr;prefix=f;target=co");
//		LogSetOptions(L"logger:///default?level=tr;prefix=f;target=fo(c:\\qwe.log)");
	}
}

class A
{
public:
	~A();
	A();
	explicit A(int a);
	A(const A & other);
	A(A && other);

	A & operator = (const A & other);
	A & operator = (A && other);

	A operator + (const A & other) const;

	void swap(A & other);

	int val() const;

private:
	int m_a;
};

A::~A()
{
	LogTraceObj();
	LogNoise(L"val: %d\n", m_a);
}

A::A(): m_a()
{
	LogTraceObj();
}

A::A(int a): m_a(a)
{
	LogTraceObj();
	LogNoise(L"%d\n", m_a);
}

A::A(const A& other): m_a(other.m_a)
{
	LogTraceObj();
}

A::A(A&& other): m_a(simstd::move(other.m_a))
{
	LogTraceObj();
}

A& A::operator =(const A& other)
{
	LogTraceObj();
	m_a = other.m_a;
	return *this;
}

A& A::operator =(A&& other)
{
	LogTraceObj();
	m_a = other.m_a;
	return *this;
}

A A::operator + (const A & /*other*/) const
{
	return A();
}

void A::swap(A& other)
{
	LogTraceObj();
	using simstd::swap;
	swap(m_a, other.m_a);
}

int A::val() const
{
	return m_a;
}

typedef typename memory::heap::DefaultStatTag<A> heap_type;
typedef typename simstd::AllocatorHeap<A, heap_type> EqAlloc;

#include <string>

extern "C" int wmain(int /*argc*/, wchar_t * /*argv*/[])
{
	setup_logger();

	LogTrace();
	std::string a;

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
		console::printf(L"stat alloc: %I64u, %I64u \n", stat.allocations, stat.allocSize);
		console::printf(L"stat free : %I64u, %I64u \n", stat.frees, stat.freeSize);
		console::printf(L"stat diff : %I64d \n", stat.allocSize - stat.freeSize);
	}

	return 0;
}
