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

		safe_snprintf(buff, lengthof(buff), L"logger:///default?level=%s;prefix=%d;target=%s", L"t2", prefix, L"co");

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

LONG WINAPI RedirectedAddVectoredExceptionHandler(ULONG /*First*/, EXCEPTION_POINTERS * /*ExceptionInfo*/)
{
	// When the CRT calls SetUnhandledExceptionFilter with NULL parameter
	// our handler will not get removed.

	console::printf("%s:%d\n", __PRETTY_FUNCTION__, __LINE__);

	return 0;
}

LONG WINAPI OurAddVectoredExceptionFilter(EXCEPTION_POINTERS * /*ExceptionInfo*/)
{
	console::printf("%s:%d\n", __PRETTY_FUNCTION__, __LINE__);

//	return EXCEPTION_CONTINUE_EXECUTION;
	return showCrashDialog ? EXCEPTION_CONTINUE_SEARCH : EXCEPTION_EXECUTE_HANDLER;
}

//LONG WINAPI VectoredExceptionHandler(PEXCEPTION_POINTERS /*pExceptionInfo*/)
//{
//	console::printf(L"%S:%d\n", __PRETTY_FUNCTION__, __LINE__);
////		std::ofstream f;
////		f.open("VectoredExceptionHandler.txt", std::ios::out | std::ios::trunc);
////		f << std::hex << pExceptionInfo->ExceptionRecord->ExceptionCode << std::endl;
////		f.close();
//
//	return EXCEPTION_CONTINUE_SEARCH;
//}

//LONG WINAPI OurSetUnhandledExceptionFilter(EXCEPTION_POINTERS * /*ExceptionInfo*/)
//{
//	console::printf("%s:%d\n", __PRETTY_FUNCTION__, __LINE__);
//
//	return showCrashDialog ? EXCEPTION_CONTINUE_SEARCH : EXCEPTION_EXECUTE_HANDLER;
//}

void my_terminate_handler()
{
	console::printf("%s:%d\n", __PRETTY_FUNCTION__, __LINE__);
}

int main(int argc, char* argv[])
{
	console::printf("%s:%d\n", __PRETTY_FUNCTION__, __LINE__);
	crt::set_unhandled_exception_filter();
//	::AddVectoredExceptionHandler(0, OurAddVectoredExceptionFilter);

	console::printf("%s:%d\n", __PRETTY_FUNCTION__, __LINE__);
	setup_logger();
	console::printf("%s:%d\n", __PRETTY_FUNCTION__, __LINE__);
//	linkage::CAPIHook apiHook1("kernel32.dll", "SetUnhandledExceptionFilter", (PROC)RedirectedSetUnhandledExceptionFilter);
//	linkage::CAPIHook apiHook2("kernel32.dll", "AddVectoredExceptionHandler", (PROC)RedirectedAddVectoredExceptionHandler);

	console::printf("%s:%d\n", __PRETTY_FUNCTION__, __LINE__);
//	console::set_output_codepage(console::Codepage::UTF8);

	LogDebug(L"argc: %d\n", argc);
	for (int i = 0; i < argc; ++i)
		LogDebug(L"'%S'\n", argv[i]);

	std::set_terminate(my_terminate_handler);

	console::printf("%s:%d\n", __PRETTY_FUNCTION__, __LINE__);
//	// Exception code c0000374
//	RaiseException(0xc0000374, 0, 0, NULL);
//
//	// Exception code c0000005
	int* p1 = NULL;
	*p1 = 99;

//	AddVectoredExceptionHandler(1, VectoredExceptionHandler);
//	SetUnhandledExceptionFilter(TopLevelExceptionHandler);

//	::AddVectoredExceptionHandler(0, OurAddVectoredExceptionFilter);
//	::SetUnhandledExceptionFilter(OurSetUnhandledExceptionFilter);

//	exception::set_vectored_filter();
//	exception::set_unhandled_filter();

	console::printf("%s:%d\n", __PRETTY_FUNCTION__, __LINE__);
	test_crashes();
	try {
//		console::printf("%s:%d\n", __PRETTY_FUNCTION__, __LINE__);
//		test_crashes();

//		test_auth();
//		add_auth();

//		test_service();
//	} catch (exception::Abstract& e) {
//		LogError(L"exception cought: %s, %s\n", e.what(), e.where());
//
//		auto mstr = e.format_error();
//		for (size_t i = 0; i < mstr.size(); ++i)
//			LogError(L"\t%s\n", mstr[i]);
//
//		return e.code();
//	} catch (std::exception& e) {
//		LogError(L"std::exception [%S]:\n", typeid(e).name());
//		LogError(L"What: %S\n", e.what());
//		return 1;
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
