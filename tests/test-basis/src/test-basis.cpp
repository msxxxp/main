#include <tests.hpp>

#include <basis/sys/console.hpp>
#include <basis/sys/logger.hpp>

namespace {
	void setup_logger()
	{
		LogSetOptions(L"logger:///default?level=tr;prefix=f;target=co");
//		LogSetOptions(L"logger:///default?level=tr;prefix=f;target=fo(c:\\qwe.log)");
	}
}

extern "C" int wmain(int argc, wchar_t* argv[])
{
	console::printf(L"%S:%d\n", __PRETTY_FUNCTION__, __LINE__);

	setup_logger();

	LogDebug(L"argc: %d\n", argc);
	for (int i = 0; i < argc; ++i)
		LogDebug(L"'%s'\n", argv[i]);

	test_logger();

//	test_chrono();
//
//	test_console_input();
//
//	test_traceback();
//
//	test_threads();
//
//	test_url();
//
//	test_window();

	return 0;
}

int WINAPI wWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, LPWSTR /*lpCmdLine*/, int /*nShowCmd*/)
{
	return wmain(0, nullptr);
}
