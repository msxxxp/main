#include <tests.hpp>

#include <basis/configure.hpp>
#include <basis/sys/logger.hpp>
#include <basis/simstd/string>

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

void test_crashes()
{
	test_null_string();

	test_zero_division();

	test_access_violation();
}
