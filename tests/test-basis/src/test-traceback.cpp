#include <tests.hpp>

#include <basis/sys/console.hpp>
#include <basis/sys/logger.hpp>
#include <basis/sys/traceback.hpp>
#include <basis/sys/totext.hpp>
#include <basis/simstd/string>

namespace local {

	LogRegisterLocal(L"print_trace");

	void init_print_trace()
	{
		LogSetOptions(L"logger:///module?name=print_trace;prefix=fu;level=in");
		LogSetOptions(L"logger:///module?name=traceback;prefix=fu;level=in");
		traceback::init();
	}

	void print_trace()
	{
		traceback::Enum tb;

		//	auto module = get_logger_module();
		//	auto scopeLock(module->lock_scope());
		LogForce(L"Backtrace: [%Iu]\n", tb.size());
		for (size_t i = 0; i < tb.size(); ++i)
			LogForce(L"[%02Iu] %s\n", tb.size() - (i + 1), tb[i].to_str().c_str());
	}

//	void print_trace(PCONTEXT context, void* address)
//	{
//		traceback::Enum tb(context, address);
//
//		LogForce(L"Backtrace: [%Iu]\n", tb.size());
//		for (size_t i = 0; i < tb.size(); ++i)
//			LogForce(L"[%02Iu] %s\n", tb.size() - (i + 1), tb[i].to_str().c_str());
//	}

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
		LogWarn(L"%d\n", p);
		UNUSED(p);
	}
}

void test_access_violation()
{
	LogTrace();
	local::do_acces_violation();
	LogTrace();
}

void test_zero_division()
{
	LogTrace();
	local::do_zero_division();
	LogTrace();
}

void test_traceback()
{
	LogTrace();

	local::init_print_trace();

	local::print_trace();

	crt::set_unhandled_exception_filter();

	test_zero_division();

	test_access_violation();
}
