#include <system/traceback.hpp>
#include <system/logger.hpp>
#include <system/totext.hpp>
#include <simstd/string>

#include <print_trace.hpp>

namespace {
	void setup_logger()
	{
		LogSetOptions(L"logger:///default?level=tr;prefix=fu;target=co");

		init_print_trace();
	}

	LONG WINAPI unhandledExceptionFilter(PEXCEPTION_POINTERS ep)
	{
		LogReport(L"code:    0x%X\n", ep->ExceptionRecord->ExceptionCode);
		LogReport(L"flags:   0x%X\n", ep->ExceptionRecord->ExceptionFlags);
		LogReport(L"record:  0x%p\n", ep->ExceptionRecord->ExceptionRecord);
		LogReport(L"address: 0x%p\n", ep->ExceptionRecord->ExceptionAddress);
		LogReport(L"params:  %u\n", ep->ExceptionRecord->NumberParameters);
		for (DWORD i = 0; i < ep->ExceptionRecord->NumberParameters; ++i) {
			LogReport(L"param[%u]:    0x%I64X\n", i, ep->ExceptionRecord->ExceptionInformation[i]);
		}

		LogFatal(L"terminating process %s\n", totext::nt_status(ep->ExceptionRecord->ExceptionCode).c_str());
		traceback::LazyFrame frame(reinterpret_cast<void*>(ep->ExceptionRecord->ExceptionAddress));
		LogFatal(L"exception at %s\n", frame.to_str().c_str());

//		print_trace(ep->ContextRecord, reinterpret_cast<void*>(ep->ExceptionRecord->ExceptionAddress));

		return EXCEPTION_EXECUTE_HANDLER; // should terminate process.
	}

	void do_acces_violation()
	{
		LogWarn(L"accessing null pointer\n");
		volatile int* p = nullptr;
		*p = 0;
	}
}

void test_backtrace()
{
	LogTrace();
	print_trace();
}

void test_access_violation()
{
	LogTrace();
	do_acces_violation();
	LogTrace();
}

int main()
{
	::SetUnhandledExceptionFilter(unhandledExceptionFilter);

	setup_logger();

//	test_backtrace();

	test_access_violation();

	LogTrace();

	return 0;
}
