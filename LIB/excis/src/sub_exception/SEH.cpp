#include <excis/exception.hpp>

#include <basis/sys/logger.hpp>
#include <basis/sys/totext.hpp>

//#ifdef DEBUG
#	include <basis/sys/traceback.hpp>
//#endif

#include "exception_pvt.hpp"

namespace exception {

	SehError::SehError(PEXCEPTION_POINTERS ep):
		m_code(ep->ExceptionRecord->ExceptionCode),
		m_address(ep->ExceptionRecord->ExceptionAddress)
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

//#ifdef DEBUG
		traceback::LazyFrame frame(reinterpret_cast<void*>(ep->ExceptionRecord->ExceptionAddress));
		LogFatal(L"exception at %s\n", frame.to_str().c_str());
//#endif
	}

	SehError * SehError::clone() const
	{
		return new SehError(*this);
	}

	ustring SehError::type() const
	{
		return L"SehError";
	}

	ustring SehError::what() const
	{
		return totext::api_error(code(), L"NTDLL.DLL");
	}

	DWORD SehError::code() const
	{
		return m_code;
	}

	void SehError::format_error(cstr::mstring & out) const
	{
		wchar_t buf[MAX_PATH_LEN] = {0};

		safe_snprintf(buf, lengthof(buf), L"Error: %s", what().c_str());
		out.push_back(buf);
//#ifndef NDEBUG
		safe_snprintf(buf, lengthof(buf), L"Exception: %s", type().c_str());
		out.push_back(buf);
		safe_snprintf(buf, lengthof(buf), L"Where: %s", where().c_str());
		out.push_back(buf);
//#endif
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
		LogTrace();
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

			throw SehError(ep);
		}
		return EXCEPTION_CONTINUE_SEARCH;
	}

	void set_vectored_exception_filter()
	{
		LogTrace();
		::AddVectoredExceptionHandler(1, vectored_exception_handler);
	}

}
