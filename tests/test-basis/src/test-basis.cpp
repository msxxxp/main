#include <tests.hpp>

#include <basis/sys/console.hpp>
#include <basis/sys/crt.hpp>
#include <basis/sys/logger.hpp>
#include <basis/sys/traceback.hpp>

namespace {
	void setup_logger()
	{
		console::printf("%s:%d\n", __PRETTY_FUNCTION__, __LINE__);

		wchar_t buff[256];

		logger::Prefix::flags prefix = logger::Prefix::Full & ~logger::Prefix::Date;

		safe_snprintf(buff, lengthof(buff), L"logger:///default?level=%s;prefix=%d;target=%s", L"t2", prefix, L"co");

		LogSetOptions(buff);

		traceback::init();
	}

}

int main(int argc, char* argv[])
{
	console::printf("%s:%d\n", __PRETTY_FUNCTION__, __LINE__);

	console::set_output_codepage(console::Codepage::UTF8);

	crt::set_unhandled_exception_filter();

	setup_logger();

	LogDebug(L"argc: %d\n", argc);
	for (int i = 0; i < argc; ++i)
		LogDebug(L"'%S'\n", argv[i]);

//	// Exception code c0000005
//	int* p1 = NULL;
//	*p1 = 99;

//	test_logger();
//
//	test_chrono();
//
//	test_console_input();
//
//	test_crypt();
//
	test_crashes();
//
//	test_traceback();
//
//	test_threads();
//
//	test_url();
//
//	test_window();
//
//	test_zodiac();

	console::printf("%s:%d\n", __PRETTY_FUNCTION__, __LINE__);
	return 0;
}

extern "C" int wmain(int /*argc*/, wchar_t* /*argv*/[])
{
	return main(0, nullptr);
}

int WINAPI wWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, LPWSTR /*lpCmdLine*/, int /*nShowCmd*/)
{
	return main(0, nullptr);
}
