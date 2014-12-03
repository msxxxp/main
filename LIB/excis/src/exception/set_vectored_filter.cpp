#include "exception_pvt.hpp"

#include <excis/exception.hpp>

#include <basis/sys/logger.hpp>
#include <basis/sys/totext.hpp>
#include <basis/sys/traceback.hpp>

#include <basis/simstd/string>

namespace exception {

	SehError::SehError(PEXCEPTION_POINTERS ep):
		m_code(ep->ExceptionRecord->ExceptionCode),
		m_address(ep->ExceptionRecord->ExceptionAddress)
	{
		LogDebug(L"ep:      %p\n", ep);
		LogReport(L"code:    0x%X\n", ep->ExceptionRecord->ExceptionCode);
		LogReport(L"flags:   0x%X\n", ep->ExceptionRecord->ExceptionFlags);
		LogReport(L"record:  0x%p\n", ep->ExceptionRecord->ExceptionRecord);
		LogReport(L"address: 0x%p\n", ep->ExceptionRecord->ExceptionAddress);
		LogDebug(L"params:  %u\n", ep->ExceptionRecord->NumberParameters);
		for (DWORD i = 0; i < ep->ExceptionRecord->NumberParameters; ++i) {
			LogDebug(L"param[%u]:    0x%I64X\n", i, ep->ExceptionRecord->ExceptionInformation[i]);
		}

		traceback::LazyFrame frame(m_address);
		change_where(frame.to_str().c_str());
//		LogFatal(L"exception at %s\n", where());
#ifdef DEBUG
		traceback::Enum bt;
		for (auto it = bt.begin(); it != bt.end(); ++it) {
			LogReport(L"\tbt: %s\n", it->to_str().c_str());
		}
#endif
	}

	SehError * SehError::clone() const
	{
		return new SehError(*this);
	}

	const wchar_t* SehError::type() const
	{
		return L"SehError";
	}

	const wchar_t* SehError::what() const
	{
		if (Abstract::what() == nullptr)
			change_what(totext::api_error(code(), L"NTDLL.DLL").c_str());
		return Abstract::what();
	}

	DWORD SehError::code() const
	{
		return m_code;
	}

	LONG WINAPI vectored_handler(PEXCEPTION_POINTERS ep)
	{
		if (ep->ExceptionRecord->ExceptionCode != 0x20474343) {
			LogDebug(L"code:    0x%X\n", ep->ExceptionRecord->ExceptionCode);
			LogDebug(L"flags:   0x%X\n", ep->ExceptionRecord->ExceptionFlags);
			LogDebug(L"record:  0x%p\n", ep->ExceptionRecord->ExceptionRecord);
			LogDebug(L"address: 0x%p\n", ep->ExceptionRecord->ExceptionAddress);
			LogDebug(L"params:  %u\n", ep->ExceptionRecord->NumberParameters);
			for (DWORD i = 0; i < ep->ExceptionRecord->NumberParameters; ++i) {
				LogDebug(L"param[%u]:    0x%I64X\n", i, ep->ExceptionRecord->ExceptionInformation[i]);
			}

			throw SehError(ep);
		}
		return EXCEPTION_CONTINUE_SEARCH;
	}

	void set_vectored_filter()
	{
		LogTrace();
		::AddVectoredExceptionHandler(1, vectored_handler);
	}

}
