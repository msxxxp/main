#include <excis/exception.hpp>

#include <basis/sys/logger.hpp>
#include <basis/sys/totext.hpp>
#include <basis/sys/traceback.hpp>

#include <basis/simstd/string>

namespace exception {

	LONG WINAPI unhandled_exception_filter(PEXCEPTION_POINTERS ep)
	{
		LogDebug(L"code:    0x%X\n", ep->ExceptionRecord->ExceptionCode);
		LogDebug(L"flags:   0x%X\n", ep->ExceptionRecord->ExceptionFlags);
		LogDebug(L"record:  0x%p\n", ep->ExceptionRecord->ExceptionRecord);
		LogDebug(L"address: 0x%p\n", ep->ExceptionRecord->ExceptionAddress);
		LogDebug(L"params:  %u\n", ep->ExceptionRecord->NumberParameters);
		for (DWORD i = 0; i < ep->ExceptionRecord->NumberParameters; ++i) {
			LogDebug(L"param[%u]:    0x%I64X\n", i, ep->ExceptionRecord->ExceptionInformation[i]);
		}

		LogFatal(L"terminating process %s\n", totext::nt_status(ep->ExceptionRecord->ExceptionCode).c_str());
		traceback::LazyFrame frame(reinterpret_cast<void*>(ep->ExceptionRecord->ExceptionAddress));
		LogFatal(L"exception at %s\n", frame.to_str().c_str());

//		return EXCEPTION_CONTINUE_EXECUTION;
		return EXCEPTION_EXECUTE_HANDLER; // should terminate process.
	}

	void set_unhandled_filter()
	{
		LogTrace();
		::SetUnhandledExceptionFilter(unhandled_exception_filter);
	}

}
