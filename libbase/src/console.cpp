#include <libbase/console.hpp>

#include <stdio.h>

namespace Base {

	size_t fileout(PCWSTR str, size_t len, HANDLE hndl)
	{
		DWORD written = 0;
		::WriteFile(hndl, str, len * sizeof(*str), &written, nullptr);
		written /= sizeof(*str);
		return written;
	}

	size_t consoleout(PCSTR str, size_t len, DWORD nStdHandle)
	{
		DWORD written = 0;
		(len && !::WriteConsoleA(::GetStdHandle(nStdHandle), str, len, &written, nullptr));
		return written;
	}

	size_t consoleout(wchar_t in, DWORD nStdHandle)
	{
		wchar_t out[] = {in, STR_END};
		return consoleout(out, nStdHandle);
	}

	int stdprintf(DWORD nStdHandle, PCWSTR format, ...)
	{
		va_list vl;
		va_start(vl, format);
		int Result = stdvprintf(nStdHandle, format, vl);
		va_end(vl);
		return Result;
	}

	int snprintf(PWSTR buff, size_t len, PCWSTR format, ...)
	{
		va_list vl;
		va_start(vl, format);
		int ret = safe_vsnprintf(buff, len, format, vl);
		va_end(vl);
		return ret;
	}

	void errx(int eval, PCSTR format, ...)
	{
		va_list vl;
		va_start(vl, format);
		::vprintf(format, vl);
		va_end(vl);
		exit(eval);
	}

//#ifndef NDEBUG
//	PCSTR FUNC_ENTER_FORMAT = "Enter >>>>>>>>    %s \t\t[%s:%d]\n";
//	PCSTR FUNC_LEAVE_FORMAT = "Leave <<<<<<<<    %s\n";
//	PCSTR FUNC_TRACE_FORMAT = "Trace =========== %s \t\t[%s:%d]\n";
//#endif

}
