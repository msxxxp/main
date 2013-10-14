#include <iostream>
using namespace std;

#include <libbase/backtrace.hpp>
#include <liblog/logger.hpp>

using namespace Base;

namespace {
	void setup_logger()
	{
		using namespace Logger;
		set_default_level(Level::Trace);
		set_default_prefix(Prefix::Medium | Prefix::Place);
		set_default_target(get_TargetToConsole());
	}
}

void test_backtrace()
{
	LogTrace();
	Base::Backtrace().Print();
}

int main()
{
	setup_logger();

	test_backtrace();

	return 0;
}
