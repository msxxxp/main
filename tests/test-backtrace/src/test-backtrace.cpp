#include <system/traceback.hpp>
#include <system/logger.hpp>
#include <simstd/string>

#include <print_trace.hpp>

namespace {
	void setup_logger()
	{
		LogSetOptions(L"logger:///default?level=tr;prefix=fu;target=co");

		init_print_trace();
	}
}

void test_backtrace()
{
	LogTrace();
	print_trace();
}

int main()
{
	setup_logger();

	test_backtrace();

	return 0;
}
