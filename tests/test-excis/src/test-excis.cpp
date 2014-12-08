#include <tests.hpp>

#include <basis/configure.hpp>
#include <basis/sys/cstr.hpp>
#include <basis/sys/linkage.hpp>
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
		wchar_t buff[256];
		logger::Prefix::flags prefix = logger::Prefix::Full & ~logger::Prefix::Date;
		safe_snprintf(buff, lengthof(buff), L"logger:///default?level=%s;prefix=%d;target=%s", L"t", prefix, L"co");
		LogSetOptions(buff);

		traceback::init();
	}
}

bool showCrashDialog = false;

LONG WINAPI RedirectedSetUnhandledExceptionFilter(EXCEPTION_POINTERS * /*ExceptionInfo*/)
{
	// When the CRT calls SetUnhandledExceptionFilter with NULL parameter
	// our handler will not get removed.

	console::printf("%s:%d\n", __PRETTY_FUNCTION__, __LINE__);

	return 0;
}

LONG WINAPI OurSetUnhandledExceptionFilter(EXCEPTION_POINTERS * /*ExceptionInfo*/)
{
	console::printf("%s:%d\n", __PRETTY_FUNCTION__, __LINE__);

	return showCrashDialog ? EXCEPTION_CONTINUE_SEARCH : EXCEPTION_EXECUTE_HANDLER;
}


int main(int argc, char* argv[])
{
	console::set_output_codepage(console::Codepage::UTF8);
	console::printf("%s:%d\n", __PRETTY_FUNCTION__, __LINE__);

	setup_logger();

	LogDebug(L"argc: %d\n", argc);
	for (int i = 0; i < argc; ++i)
		LogDebug(L"'%S'\n", argv[i]);

//	exception::set_vectored_filter();
//	exception::set_unhandled_filter();

	::SetUnhandledExceptionFilter(OurSetUnhandledExceptionFilter);

	linkage::CAPIHook apiHook("kernel32.dll", "SetUnhandledExceptionFilter", (PROC)RedirectedSetUnhandledExceptionFilter);


	try {
		test_crashes();

//		test_auth();
//		add_auth();

//		test_service();
	} catch (exception::Abstract& e) {
		LogError(L"exception cought: %s, %s\n", e.what(), e.where());

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
