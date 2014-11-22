#include <tests.hpp>

#include <basis/configure.hpp>
#include <basis/sys/logger.hpp>
#include <basis/sys/totext.hpp>
#include <basis/sys/traceback.hpp>
#include <basis/simstd/string>
#include <excis/exception.hpp>

#include <exception>
#include <typeinfo>

namespace {
	void setup_logger()
	{
		LogSetOptions(L"logger:///default?level=d;prefix=fu;target=co");

		traceback::init();
	}
}

extern "C" int wmain(int argc, wchar_t* argv[])
{
	console::printf(L"%S:%d\n", __PRETTY_FUNCTION__, __LINE__);

	setup_logger();

	LogDebug(L"argc: %d\n", argc);
	for (int i = 0; i < argc; ++i)
		LogDebug(L"'%s'\n", argv[i]);

	exception::set_vectored_exception_filter();
	exception::set_unhandled_exception_filter();

	try {
		LogTrace();
		test_crashes();

		test_auth();
//		add_auth();

		test_service();
	} catch (exception::AbstractError& e) {
		LogError(L"exception cought: %s, %s\n", e.what().c_str(), e.where().c_str());

		auto mstr = e.format_error();
		for (size_t i = 0; i < mstr.size(); ++i)
			LogError(L"\t%s\n", mstr[i]);

		return e.code();
	} catch (std::exception& e) {
		LogError(L"std::exception [%S]:\n", typeid(e).name());
		LogError(L"What: %S\n", e.what());
		return 1;
	} catch (...) {
		LogError(L"cpp exception cought\n");
	}

	return 0;
}

int WINAPI wWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, LPWSTR /*lpCmdLine*/, int /*nShowCmd*/)
{
	return wmain(0, nullptr);
}
