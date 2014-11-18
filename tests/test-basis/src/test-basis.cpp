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

int WINAPI wWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, LPWSTR /*lpCmdLine*/, int /*nShowCmd*/)
{
	console::printf(L"%S:%d\n", __PRETTY_FUNCTION__, __LINE__);

	setup_logger();

	test_list();

	return 0;
}

extern "C" int wmain(int /*argc*/, wchar_t * /*argv*/[])
{
	console::printf(L"%S:%d\n", __PRETTY_FUNCTION__, __LINE__);

	setup_logger();

	test_list();

	return 0;
}
