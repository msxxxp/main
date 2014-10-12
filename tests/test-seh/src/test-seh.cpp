#include <basis/sys/console.hpp>
#include <basis/sys/logger.hpp>
#include <basis/sys/totext.hpp>
#include <basis/sys/traceback.hpp>

#include <excis/exception.hpp>

#include <basis/simstd/string>

/// WARNING for x64 Optimization must be -O0 or it will crash (tdm compiler)

namespace {
	void setup_logger()
	{
		LogSetOptions(L"logger:///default?level=tr;prefix=fu;target=co");

		traceback::init();
	}

	void do_cpp_exception()
	{
		LogWarn(L"throwning 42\n");
		throw 42;
	}

	void do_av_exception()
	{
		LogWarn(L"accessing null pointer\n");
		volatile int* p = nullptr;
		*p = 0;
	}

	void do_division_by_zero()
	{
		LogWarn(L"divide by zero\n");
		volatile int n = 1;
		volatile int d = 0;
		volatile int r = n / d;

		UNUSED(r);
	}
}

extern "C" int wmain(int argc, wchar_t * argv[])
{
	UNUSED(argc);
	UNUSED(argv);

	console::printf(L"%S:%d\n", __PRETTY_FUNCTION__, __LINE__);

	setup_logger();

	exception::set_vectored_exception_filter();
	exception::set_unhandled_exception_filter();

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

	try {
		do_av_exception();
	} catch (exception::AbstractError & e) {
		LogFatal(L"SEH exception cought: %s\n", e.what().c_str());
		LogFatal(L"SEH exception cought: %s\n", e.where().c_str());

		auto mstr = e.format_error();
		for (size_t i = 0; i < mstr.size(); ++i)
			LogFatal(L"\t%s\n", mstr[i]);
	} catch (...) {
		LogFatal(L"cpp exception cought\n");
	}

	try {
		do_division_by_zero();
	} catch (exception::AbstractError & e) {
		LogFatal(L"SEH exception cought: %s\n", e.what().c_str());
		LogFatal(L"SEH exception cought: %s\n", e.where().c_str());

		auto mstr = e.format_error();
		for (size_t i = 0; i < mstr.size(); ++i)
			LogFatal(L"\t%s\n", mstr[i]);
	} catch (...) {
		LogFatal(L"cpp exception cought\n");
	}

	return 0;
}

int WINAPI wWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, LPWSTR /*lpCmdLine*/, int /*nShowCmd*/)
{
	return wmain(0, nullptr);
}
