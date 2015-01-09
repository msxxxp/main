#include <tests.hpp>

#include <basis/configure.hpp>
#include <basis/sys/logger.hpp>
#include <basis/simstd/string>

#include <ctime>

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
	*p = 42;
}

void do_zero_division()
{
	LogWarn(L"division by zero\n");
	int zero = 0;
	int p = 42 / zero;
	LogWarn(L"%d\n", p);
	UNUSED(p);
}

void crash_null_string()
{
	do_null_string();
}

void crash_access_violation()
{
	do_acces_violation();
}

void crash_zero_division()
{
	do_zero_division();
}

void crash_virtual_function_call()
{
	LogWarn(L"pure virtual function call\n");
	struct B
	{
		virtual ~B() = default;

		B()
		{
			LogTrace();
			nvf();
		}

		virtual void vf() = 0;

		void nvf() {vf();}
	};

	struct D: public B
	{
		void vf() override {LogTrace();}
	};

	B* b = new D;
	b->vf();
}

/// WARNING for x64 Optimization must be -O0 or it will crash (tdm compiler)

void test_crashes()
{
	srand(static_cast<unsigned int>(std::time(0)));
	int option = rand() % 5;

	switch (option)
	{
		case 0: // no crash
			break;
		case 1:
			crash_null_string();
			break;
		case 2:
			crash_zero_division();
			break;
		case 3:
			crash_access_violation();
			break;
		case 4:
			crash_virtual_function_call();
			break;
	}
}
