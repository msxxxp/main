#include <tests.hpp>

#include <basis/sys/console.hpp>
#include <basis/sys/crt.hpp>
#include <basis/sys/logger.hpp>

namespace {
	void setup_logger()
	{
		TraceFunc();

		wchar_t buff[256];

		logger::Prefix::flags prefix = logger::Prefix::Full & ~logger::Prefix::Date;

		safe_snprintf(buff, lengthof(buff), L"logger:///default?level=%s;prefix=%d;target=%s", L"t1", prefix, L"co");

		LogSetOptions(buff);
		TraceFunc();
	}
}

int main(int argc, char* argv[])
{
	TraceFunc();
	console::set_output_codepage(console::Codepage::UTF8);

	TraceFunc();
	setup_logger();

	LogDebug(L"argc: %d\n", argc);
	for (int i = 0; i < argc; ++i)
		LogDebug(L"'%S'\n", argv[i]);

//	crt::set_unhandled_exception_filter();
	crt::set_vectored_exception_filter();
//
//	test_logger();
//
//	test_chrono();
//
//	test_console_input();
//
//	test_crypt();
//
//	test_crashes();
//
	test_list();
//
//	test_math();
//
//	test_memory();
//
//	test_string();
//
//	test_traceback();
//
//	test_threads();
//
//	test_url();
//
//	test_vector();
//
//	test_window();
//
//	test_zodiac();

	TraceFunc();
	return 0;
}

extern "C" int wmain(int /*argc*/, wchar_t* /*argv*/[])
{
	TraceFunc();
	return main(0, nullptr);
}

int WINAPI wWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, LPWSTR /*lpCmdLine*/, int /*nShowCmd*/)
{
	TraceFunc();
	return main(0, nullptr);
}
