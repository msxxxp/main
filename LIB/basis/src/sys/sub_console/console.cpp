#include <basis/sys/console.hpp>

//	size_t fileout(const wchar_t * str, size_t len, HANDLE hndl)
//	{
//		DWORD written = 0;
//		::WriteFile(hndl, str, len * sizeof(*str), &written, nullptr);
//		written /= sizeof(*str);
//		return written;
//	}

//#ifndef NDEBUG
//	const char * FUNC_ENTER_FORMAT = "Enter >>>>>>>>    %s \t\t[%s:%d]\n";
//	const char * FUNC_LEAVE_FORMAT = "Leave <<<<<<<<    %s\n";
//	const char * FUNC_TRACE_FORMAT = "Trace =========== %s \t\t[%s:%d]\n";
//#endif
