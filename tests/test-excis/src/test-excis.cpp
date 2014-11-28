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
		LogSetOptions(L"logger:///default?level=t3;prefix=fu;target=co");

		traceback::init();
	}
}

int main(int argc, char* argv[])
{
	console::set_output_codepage(console::Codepage::UTF8);
	console::printf("%s:%d\n", __PRETTY_FUNCTION__, __LINE__);

	setup_logger();

	LogDebug(L"argc: %d\n", argc);
	for (int i = 0; i < argc; ++i)
		LogDebug(L"'%S'\n", argv[i]);

	exception::set_vectored_exception_filter();
	exception::set_unhandled_exception_filter();

	try {
//		test_crashes();

//		test_auth();
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
