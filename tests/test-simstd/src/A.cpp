#include "A.hpp"

#include <system/console.hpp>

class A::implementation
{
public:
	~implementation()
	{
		console::printf(L"%S\n", __PRETTY_FUNCTION__);
	}

	implementation()
	{
		console::printf(L"%S\n", __PRETTY_FUNCTION__);
	}
};

A::~A()
{
	console::printf(L"%S\n", __PRETTY_FUNCTION__);
}

A::A() :
	_imp(new implementation)
{
	console::printf(L"%S\n", __PRETTY_FUNCTION__);
}

void A::do_something()
{
	console::printf(L"%S use_count() is %Iu\n", __PRETTY_FUNCTION__, _imp.use_count());
}

A * get_A()
{
	return new A;
}

class B
{
public:
	~B()
	{
		console::printf(L"%S\n", __PRETTY_FUNCTION__);
	}

	B()
	{
		console::printf(L"%S\n", __PRETTY_FUNCTION__);
	}
};

B * get_B()
{
	return new B;
}

void destroy_B(B * b)
{
	delete b;
}
