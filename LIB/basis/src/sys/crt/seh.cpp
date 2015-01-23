#include <basis/sys/crt.hpp>
#include <basis/sys/console.hpp>
#include <basis/sys/totext.hpp>
#include <basis/sys/traceback.hpp>

#include <basis/simstd/string>

namespace crt {

	LONG WINAPI unhandled_exception_filter(PEXCEPTION_POINTERS ep)
	{
		console::printf("%s(): code:    0x%X\n", __FUNCTION__, ep->ExceptionRecord->ExceptionCode);
		console::printf("%s(): flags:   0x%X\n", __FUNCTION__, ep->ExceptionRecord->ExceptionFlags);
		console::printf("%s(): record:  0x%p\n", __FUNCTION__, ep->ExceptionRecord->ExceptionRecord);
		console::printf("%s(): address: 0x%p\n", __FUNCTION__, ep->ExceptionRecord->ExceptionAddress);
		console::printf("%s(): params:  %u\n", __FUNCTION__, ep->ExceptionRecord->NumberParameters);
		for (DWORD i = 0; i < ep->ExceptionRecord->NumberParameters; ++i) {
			console::printf("%s(): param[%u]:    0x%I64X\n", __FUNCTION__, i, ep->ExceptionRecord->ExceptionInformation[i]);
		}

//		traceback::LazyFrame frame(reinterpret_cast<void*>(ep->ExceptionRecord->ExceptionAddress));
//		console::printf("%s(): exception at %S\n", __FUNCTION__, frame.to_str().c_str());

		traceback::Enum tb(ep->ContextRecord, ep->ExceptionRecord->ExceptionAddress);
		console::printf("Backtrace: [%Iu]\n", tb.size());
		for (size_t i = 0; i < tb.size(); ++i)
			console::printf("[%02Iu] %S\n", tb.size() - (i + 1), tb[i].to_str().c_str());

//		print_trace(ep->ContextRecord, reinterpret_cast<void*>(ep->ExceptionRecord->ExceptionAddress));

		console::printf("%s(): terminating process\n", __FUNCTION__);
		console::printf("%s(): with error %S\n", __FUNCTION__, totext::nt_status(ep->ExceptionRecord->ExceptionCode).c_str());

		::ExitProcess(ep->ExceptionRecord->ExceptionCode);
//		return EXCEPTION_CONTINUE_EXECUTION; // should terminate process.
		return EXCEPTION_EXECUTE_HANDLER;    // should terminate process.
	}

	void set_unhandled_exception_filter()
	{
		traceback::init();
		::SetUnhandledExceptionFilter(unhandled_exception_filter);
	}

	LONG WINAPI vectored_exception_filter(PEXCEPTION_POINTERS ep)
	{
		UNUSED(ep);
		return EXCEPTION_EXECUTE_HANDLER;
	}

	void set_vectored_exception_filter()
	{
		traceback::init();
		::AddVectoredExceptionHandler(1, unhandled_exception_filter);
	}

}
