#include <tests.hpp>

#include <basis/configure.hpp>
#include <basis/sys/logger.hpp>
#include <basis/simstd/string>

#include <excis/exception.hpp>

void do_cpp_exception()
{
	LogWarn(L"throwning 42\n");
	throw 42;
}

void do_null_string()
{
	LogWarn(L"length null sting\n");
	const wchar_t* str = nullptr;
	wcslen(str);
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
	LogWarn(L"%d\n", p);
	UNUSED(p);
}

void test_null_string()
{
	LogTrace();
	do_null_string();
	LogTrace();
}

void test_cpp_exception()
{
	LogTrace();

	try {
		do_cpp_exception();
	} catch (exception::AbstractError & e) {
		LogFatal(L"SEH exception cought: %s\n", e.what().c_str());
		LogFatal(L"SEH exception cought: %s\n", e.where().c_str());

		auto mstr = e.format_error();
		for (size_t i = 0; i < mstr.size(); ++i)
			LogFatal(L"\t%s\n", mstr[i]);
	} catch (...) {
		LogFatal(L"cpp exception cought\n");
	}

	LogTrace();
}

void test_access_violation()
{
	LogTrace();

	try {
		do_acces_violation();
	} catch (exception::AbstractError & e) {
		LogFatal(L"SEH exception cought: %s\n", e.what().c_str());
		LogFatal(L"SEH exception cought: %s\n", e.where().c_str());

		auto mstr = e.format_error();
		for (size_t i = 0; i < mstr.size(); ++i)
			LogFatal(L"\t%s\n", mstr[i]);
	} catch (...) {
		LogFatal(L"cpp exception cought\n");
	}

	LogTrace();
}

void test_zero_division()
{
	LogTrace();

	try {
		do_zero_division();
	} catch (exception::AbstractError & e) {
		LogFatal(L"SEH exception cought: %s\n", e.what().c_str());
		LogFatal(L"SEH exception cought: %s\n", e.where().c_str());

		auto mstr = e.format_error();
		for (size_t i = 0; i < mstr.size(); ++i)
			LogFatal(L"\t%s\n", mstr[i]);
	} catch (...) {
		LogFatal(L"cpp exception cought\n");
	}

	LogTrace();
}

/// WARNING for x64 Optimization must be -O0 or it will crash (tdm compiler)

void test_crashes()
{
//	test_null_string();

	test_zero_division();

	test_access_violation();
}
