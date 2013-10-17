#include <libbase/console.hpp>

#include <stdio.h>

namespace Base {
	namespace Console {
		size_t out(wchar_t in, Handle hnd)
		{
			wchar_t str[] = {in, STR_END_C};
			return out(str, hnd);
		}

		size_t out(PCSTR str, size_t len, Handle hnd)
		{
			DWORD written = 0;
			(len && !::WriteConsoleA(::GetStdHandle((DWORD)hnd), str, len, &written, nullptr));
			return written;
		}

		int printf(Handle hnd, PCWSTR format, ...)
		{
			va_list vl;
			va_start(vl, format);
			int Result = vprintf(hnd, format, vl);
			va_end(vl);
			return Result;
		}

	}

	size_t fileout(PCWSTR str, size_t len, HANDLE hndl)
	{
		DWORD written = 0;
		::WriteFile(hndl, str, len * sizeof(*str), &written, nullptr);
		written /= sizeof(*str);
		return written;
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
