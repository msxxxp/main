#include <tests.hpp>

#include <basis/configure.hpp>
#include <basis/sys/logger.hpp>
#include <basis/simstd/string>

#include <excis/exception.hpp>

#include <ctime>

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

void crash_null_string()
{
	LogTrace();
	try {
		do_null_string();
	} catch (exception::Abstract & e) {
		auto mstr = e.format_error();
		for (size_t i = 0; i < mstr.size(); ++i)
			LogFatal(L"\t%s\n", mstr[i]);
	} catch (...) {
		LogFatal(L"cpp exception cought\n");
	}
	LogTrace();
}

void crash_cpp_exception()
{
	LogTrace();

	try {
		do_cpp_exception();
	} catch (exception::Abstract & e) {
		auto mstr = e.format_error();
		for (size_t i = 0; i < mstr.size(); ++i)
			LogFatal(L"\t%s\n", mstr[i]);
	} catch (...) {
		LogFatal(L"cpp exception cought\n");
	}

	LogTrace();
}

void crash_access_violation()
{
	LogTrace();
	try {
		do_acces_violation();
	} catch (exception::Abstract & e) {
		auto mstr = e.format_error();
		for (size_t i = 0; i < mstr.size(); ++i)
			LogFatal(L"\t%s\n", mstr[i]);
	} catch (...) {
		LogFatal(L"cpp exception cought\n");
	}
	LogTrace();
}

void crash_zero_division()
{
	LogTrace();
	try {
		do_zero_division();
	} catch (exception::Abstract & e) {
		auto mstr = e.format_error();
		for (size_t i = 0; i < mstr.size(); ++i)
			LogFatal(L"\t%s\n", mstr[i]);
	} catch (...) {
		LogFatal(L"cpp exception cought\n");
	}
	LogTrace();
}

void crash_virtual_function_call()
{
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
	int option = rand() % 6;

	switch (option)
	{
		case 0:
			crash_null_string();
			break;
		case 1:
			crash_zero_division();
			break;
		case 2:
			crash_access_violation();
			break;
		case 3:
			crash_cpp_exception();
			break;
		case 4:
			crash_cpp_exception();
			break;
		case 5:
			crash_virtual_function_call();
			break;
	}
}
