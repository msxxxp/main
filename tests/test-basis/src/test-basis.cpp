#include <basis/sys/logger.hpp>

//#include <algorithm>
//#include <vector>
#include <basis/simstd/list>
#include <basis/std/algorithm>

namespace {
	void setup_logger()
	{
		LogSetOptions(L"logger:///default?level=tr;prefix=fu;target=co");
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

private:
	int m_a;
};

A::~A()
{
	LogTraceObj();
}

A::A(): m_a()
{
	LogTraceObj();
}

A::A(int a): m_a(a)
{
	LogTraceObj();
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

extern "C" int wmain(int /*argc*/, wchar_t * /*argv*/[])
{
	setup_logger();

	LogTrace();
	A ma[7];

	LogTrace();

	A a;
	simstd::list<A> list1;
	LogInfo(L"1:size(): %Iu\n", list1.size());
	LogInfo(L"1:empty(): %d\n", list1.empty());

	simstd::list<A> list2(5, a);
	LogInfo(L"2:size(): %Iu\n", list2.size());
	LogInfo(L"2:empty(): %d\n", list2.empty());

	simstd::list<A> list3(10);
	LogInfo(L"3:size(): %Iu\n", list3.size());
	LogInfo(L"3:empty(): %d\n", list3.empty());

	simstd::list<A> list4(simstd::begin(ma), simstd::end(ma));
	LogInfo(L"4:size(): %Iu\n", list4.size());
	LogInfo(L"4:empty(): %d\n", list4.empty());

	simstd::list<A> list4a(list4.begin(), list4.end());
	LogInfo(L"4a:size(): %Iu\n", list4a.size());
	LogInfo(L"4a:empty(): %d\n", list4a.empty());

	simstd::list<A> list5(list2);
	LogInfo(L"5:size(): %Iu\n", list5.size());
	LogInfo(L"5:empty(): %d\n", list5.empty());

	simstd::list<A> list6(simstd::move(list3));
	LogInfo(L"6:size(): %Iu\n", list6.size());
	LogInfo(L"6:empty(): %d\n", list6.empty());
	LogInfo(L"3:size(): %Iu\n", list3.size());
	LogInfo(L"3:empty(): %d\n", list3.empty());

//	list1.emplace_back(a);

//	list1.reserve(8);

//	list1.push_back(a);
//
//	list1.push_back(a + a);
//
//	list1.push_back(simstd::move(a));

	return 0;
}
