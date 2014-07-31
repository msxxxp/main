#include <basis/sys/logger.hpp>

#include <algorithm>
#include <vector>

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

A::A(const A& other): m_a(other.m_a)
{
	LogTraceObj();
}

A::A(A&& other): m_a(other.m_a)
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
	using std::swap;
	swap(m_a, other.m_a);
}

extern "C" int wmain(int /*argc*/, wchar_t * /*argv*/[])
{
	setup_logger();

	LogTrace();

	std::vector<A> vec1;
	vec1.reserve(8);

	A a;
	vec1.push_back(a);

	vec1.push_back(a + a);

	vec1.push_back(std::move(a));

	return 0;
}
