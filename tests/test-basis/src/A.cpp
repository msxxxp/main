#include <A.hpp>
#include <basis/sys/logger.hpp>
#include <basis/simstd/algorithm>

A::~A()
{
	LogTraceObj();
	LogNoise2(L"val: %d\n", m_a);
}

A::A() :
	m_a()
{
	LogTraceObj();
}

A::A(int a) :
	m_a(a)
{
	LogTraceObj();
	LogNoise2(L"%d\n", m_a);
}

A::A(const A& other) :
	m_a(other.m_a)
{
	LogTraceObj();
}

A::A(A&& other) :
	m_a(simstd::move(other.m_a))
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

A A::operator +(const A & /*other*/) const
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
