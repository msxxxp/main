#include <basis/sys/traceback.hpp>
#include <basis/sys/logger.hpp>
#include <basis/sys/totext.hpp>
#include <basis/simstd/string>

#include <print_trace.hpp>

namespace {
	void setup_logger()
	{
		LogSetOptions(L"logger:///default?level=tr;prefix=fu;target=co");

		init_print_trace();
	}

	void do_acces_violation()
	{
		LogWarn(L"accessing null pointer\n");
		volatile int* p = nullptr;
		*p = 0;
	}

	void do_zero_division()
	{
		LogWarn(L"division by zero\n");
		int zero = 0;
		int p = 42 / zero;
		UNUSED(p);
	}
}

void test_backtrace()
{
	LogTrace();
	print_trace();
}

void test_access_violation()
{
	LogTrace();
	do_acces_violation();
	LogTrace();
}

void test_zero_division()
{
	LogTrace();
	do_zero_division();
	LogTrace();
}

int main()
{
	crt::set_unhandled_exception_filter();

	setup_logger();

	test_backtrace();

	test_zero_division();

	test_access_violation();

	return 0;
}
