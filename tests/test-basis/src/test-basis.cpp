#include <tests.hpp>

#include <basis/sys/console.hpp>
#include <basis/sys/logger.hpp>

namespace {
	void setup_logger()
	{
		console::set_output_codepage(console::Codepage::UTF8);
		LogSetOptions(L"logger:///default?level=to;prefix=f;target=co");
//		LogSetOptions(L"logger:///default?level=tr;prefix=f;target=fo(c:\\qwe.log)");
	}
}

int main(int argc, char* argv[])
{
	console::printf("%s:%d\n", __PRETTY_FUNCTION__, __LINE__);

	setup_logger();

	LogDebug(L"argc: %d\n", argc);
	for (int i = 0; i < argc; ++i)
		LogDebug(L"'%S'\n", argv[i]);

//	test_logger();
//
//	test_chrono();
//
//	test_console_input();
//
	test_crypt();
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
