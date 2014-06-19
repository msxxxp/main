#include <basis/sys/configure.hpp>
#include <basis/sys/console.hpp>
#include <basis/sys/logger.hpp>
#include <basis/sys/totext.hpp>

#ifdef DEBUG
#	include <basis/sys/traceback.hpp>
#endif

#include <basis/std/string>

class SehException {
public:
	SehException(PEXCEPTION_POINTERS ep)
	{
		LogDebug(L"ep: %p\n", ep);
		LogReport(L"code:    0x%X\n", ep->ExceptionRecord->ExceptionCode);
		LogReport(L"flags:   0x%X\n", ep->ExceptionRecord->ExceptionFlags);
		LogReport(L"record:  0x%p\n", ep->ExceptionRecord->ExceptionRecord);
		LogReport(L"address: 0x%p\n", ep->ExceptionRecord->ExceptionAddress);
		LogReport(L"params:  %u\n", ep->ExceptionRecord->NumberParameters);
		for (DWORD i = 0; i < ep->ExceptionRecord->NumberParameters; ++i) {
			LogReport(L"param[%u]:    0x%I64X\n", i, ep->ExceptionRecord->ExceptionInformation[i]);
		}

#ifdef DEBUG
		traceback::LazyFrame frame(reinterpret_cast<void*>(ep->ExceptionRecord->ExceptionAddress));
		LogFatal(L"exception at %s\n", frame.to_str().c_str());
#endif
	}
};

namespace {
	void setup_logger()
	{
		LogSetOptions(L"logger:///default?level=tr;prefix=fu;target=co");

#ifdef DEBUG
		traceback::init();
#endif
	}

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
//		traceback::LazyFrame frame(reinterpret_cast<void*>(ep->ExceptionRecord->ExceptionAddress));
//		LogFatal(L"exception at %s\n", frame.to_str().c_str());

//		print_trace(ep->ContextRecord, reinterpret_cast<void*>(ep->ExceptionRecord->ExceptionAddress));

//		return EXCEPTION_CONTINUE_EXECUTION;
		return EXCEPTION_EXECUTE_HANDLER; // should terminate process.
	}

	void set_unhandled_exception_filter()
	{
		::SetUnhandledExceptionFilter(unhandled_exception_filter);
	}

	LONG WINAPI vectored_exception_handler(::PEXCEPTION_POINTERS ep)
	{
		if (ep->ExceptionRecord->ExceptionCode != 0x20474343) {
			LogReport(L"entering vectorized ex. handler 0x%lx:\n", ep->ExceptionRecord->ExceptionCode);
			LogReport(L"code:    0x%X\n", ep->ExceptionRecord->ExceptionCode);
			LogReport(L"flags:   0x%X\n", ep->ExceptionRecord->ExceptionFlags);
			LogReport(L"record:  0x%p\n", ep->ExceptionRecord->ExceptionRecord);
			LogReport(L"address: 0x%p\n", ep->ExceptionRecord->ExceptionAddress);
			LogReport(L"params:  %u\n", ep->ExceptionRecord->NumberParameters);
			for (DWORD i = 0; i < ep->ExceptionRecord->NumberParameters; ++i) {
				LogReport(L"param[%u]:    0x%I64X\n", i, ep->ExceptionRecord->ExceptionInformation[i]);
			}

			throw SehException(ep);
		}
		return EXCEPTION_CONTINUE_SEARCH;
	}

	void set_vectored_exception_filter()
	{
		::AddVectoredExceptionHandler(1, vectored_exception_handler);
	}

	void do_cpp_exception()
	{
		LogWarn(L"throwning 42\n");
		throw 42;
	}

	void do_av_exception()
	{
		LogWarn(L"accessing null pointer\n");
		volatile int* p = nullptr;
		*p = 0;
	}
}

extern "C" int wmain(int argc, wchar_t * argv[])
{
	UNUSED(argc);
	UNUSED(argv);

	setup_logger();

	set_vectored_exception_filter();
	set_unhandled_exception_filter();

	try {
		do_cpp_exception();
	} catch (SehException & e) {
		LogTrace();
	} catch (...) {
		LogTrace();
	}

	try {
		do_av_exception();
	} catch (SehException & e) {
		LogTrace();
	} catch (...) {
		LogTrace();
	}
	return 0;
}
