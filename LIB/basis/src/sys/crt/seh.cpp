#include <basis/sys/crt.hpp>
#include <basis/sys/console.hpp>
#include <basis/sys/totext.hpp>
#include <basis/sys/traceback.hpp>

#include <basis/simstd/string>

namespace crt {

	LONG WINAPI unhandled_exception_filter(PEXCEPTION_POINTERS ep)
	{
		console::printf("code:    0x%X\n", ep->ExceptionRecord->ExceptionCode);
		console::printf("flags:   0x%X\n", ep->ExceptionRecord->ExceptionFlags);
		console::printf("record:  0x%p\n", ep->ExceptionRecord->ExceptionRecord);
		console::printf("address: 0x%p\n", ep->ExceptionRecord->ExceptionAddress);
		console::printf("params:  %u\n", ep->ExceptionRecord->NumberParameters);
		for (DWORD i = 0; i < ep->ExceptionRecord->NumberParameters; ++i) {
			console::printf("param[%u]:    0x%I64X\n", i, ep->ExceptionRecord->ExceptionInformation[i]);
		}

		traceback::LazyFrame frame(reinterpret_cast<void*>(ep->ExceptionRecord->ExceptionAddress));
		console::printf("exception at %S\n", frame.to_str().c_str());

//		print_trace(ep->ContextRecord, reinterpret_cast<void*>(ep->ExceptionRecord->ExceptionAddress));

		console::printf("terminating process %S\n", totext::nt_status(ep->ExceptionRecord->ExceptionCode).c_str());
//		return EXCEPTION_CONTINUE_EXECUTION; // should terminate process.
		return EXCEPTION_EXECUTE_HANDLER;    // should terminate process.
	}

	void set_unhandled_exception_filter()
	{
		traceback::init();
		::SetUnhandledExceptionFilter(unhandled_exception_filter);
	}
}
