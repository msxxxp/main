#include <tests.hpp>

#include <basis/sys/console.hpp>
#include <basis/sys/crt.hpp>
#include <basis/sys/logger.hpp>

namespace {
	void setup_logger()
	{
		console::printf("%s():%d\n", __PRETTY_FUNCTION__, __LINE__);

		wchar_t buff[256];

		logger::Prefix::flags prefix = logger::Prefix::Full & ~logger::Prefix::Date;

		safe_snprintf(buff, lengthof(buff), L"logger:///default?level=%s;prefix=%d;target=%s", L"t1", prefix, L"co");

		LogSetOptions(buff);
	}
}

int main(int argc, char* argv[])
{
	console::set_output_codepage(console::Codepage::UTF8);

	console::printf("%s():%d\n", __PRETTY_FUNCTION__, __LINE__);

	setup_logger();

	LogDebug(L"argc: %d\n", argc);
	for (int i = 0; i < argc; ++i)
		LogDebug(L"'%S'\n", argv[i]);

	crt::set_unhandled_exception_filter();

	test_logger();

//	test_chrono();
//
//	test_console_input();
//
//	test_crypt();
//
//	test_crashes();
//
//	test_list();
//
//	test_memory();
//
	test_string();
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

	console::printf("%s():%d\n", __PRETTY_FUNCTION__, __LINE__);
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
