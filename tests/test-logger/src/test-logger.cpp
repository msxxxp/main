#include <basis/sys/console.hpp>
#include <basis/sys/crt.hpp>
#include <basis/sys/logger.hpp>
#include <basis/sys/sstr.hpp>
#include <basis/sys/totext.hpp>

#include <basis/simstd/algorithm>
#include <basis/simstd/vector>
#include <basis/simstd/string>

namespace {
	void setup_logger()
	{
		LogSetOptions(L"logger:///default?level=tr;prefix=fu;target=co");
//		LogSetOptions(L"logger:///default?level=tr;prefix=fu;target=fo(test-threads.log)");

		LogTrace();
		LogDebug(L"\n");
		LogInfo(L"\n");
		LogReport(L"\n");
		LogAtten(L"\n");
		LogWarn(L"\n");
		LogError(L"\n");
		LogFatal(L"\n");
		LogAlert(L"\n");
		LogEmerg(L"\n");
		LogForce(L"\n");
	}
}

int WINAPI wWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, LPWSTR /*lpCmdLine*/, int /*nShowCmd*/)
{
	console::printf(L"%S:%d\n", __PRETTY_FUNCTION__, __LINE__);

	setup_logger();

	return 0;
}

extern "C" int wmain(int /*argc*/, wchar_t * /*argv*/[])
{
	console::printf(L"%S:%d\n", __PRETTY_FUNCTION__, __LINE__);

	setup_logger();

	return 0;
}
