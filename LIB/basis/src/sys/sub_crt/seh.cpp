#include <basis/sys/crt.hpp>
#include <basis/sys/logger.hpp>
#include <basis/sys/totext.hpp>
#include <basis/sys/traceback.hpp>

#include <basis/std/string>

namespace crt {

	LONG WINAPI unhandled_exception_filter(PEXCEPTION_POINTERS ep)
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

//		return EXCEPTION_CONTINUE_EXECUTION; // should terminate process.
		return EXCEPTION_EXECUTE_HANDLER; // should terminate process.
	}

	void set_unhandled_exception_filter()
	{
		::SetUnhandledExceptionFilter(unhandled_exception_filter);
	}
}
