#ifndef A_HPP_
#define A_HPP_

#include <simstd/memory>
#include <memory>

class A
{
public:
	~A();

	A();

	void do_something();

private:
	class implementation;
	simstd::shared_ptr<implementation> _imp;
};

#endif
