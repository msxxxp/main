#ifndef _LIBBASE_CONSOLE_HPP_
#define _LIBBASE_CONSOLE_HPP_

#include <libbase/std.hpp>
#include <libbase/pcstr.hpp>

namespace Base {

	size_t fileout(PCSTR str, size_t len, HANDLE hndl);

	size_t fileout(PCWSTR str, size_t len, HANDLE hndl);

	size_t fileout(wchar_t ch, size_t len, HANDLE hndl);

	size_t consoleout(PCSTR in, size_t len, DWORD nStdHandle = STD_OUTPUT_HANDLE );

	size_t consoleout(PCWSTR in, size_t len, DWORD nStdHandle = STD_OUTPUT_HANDLE );

	size_t consoleout(wchar_t ch, DWORD nStdHandle = STD_OUTPUT_HANDLE );

	inline int consoleout(PCWSTR str, DWORD nStdHandle = STD_OUTPUT_HANDLE )
	{
		return consoleout(str, Str::length(str), nStdHandle);
	}

	///================================================================================= Console out
	int safe_vsnprintf(PWSTR buf, size_t len, PCWSTR format, va_list vl);

	int snprintf(PWSTR buff, size_t len, PCWSTR format, ...);

	int printf(PCWSTR format, ...);

	size_t stdvprintf(DWORD nStdHandle, PCWSTR format, va_list vl);

	int stdprintf(DWORD nStdHandle, PCWSTR format, ...);

	inline int vprintf(PCWSTR format, va_list vl)
	{
		return stdvprintf(STD_OUTPUT_HANDLE, format, vl);
	}

	void errx(int eval, PCSTR format, ...);

	///================================================================================ ConsoleColor
	struct ConsoleColor {
		~ConsoleColor();

		ConsoleColor(WORD color);

		void restore();

	private:
		bool save();

		WORD m_color;
	};

/*
 #ifndef NDEBUG
 extern PCSTR FUNC_ENTER_FORMAT;
 extern PCSTR FUNC_LEAVE_FORMAT;
 extern PCSTR FUNC_TRACE_FORMAT;

 #define FuncLogger() struct FL_struc__ { \
	FL_struc__(const char * fl, int l, const char * f):_fn(f) {printf(FUNC_ENTER_FORMAT, _fn, fl, l);} \
	~FL_struc__() {printf(FUNC_LEAVE_FORMAT, _fn);} \
	const char * _fn; \
} tmp_struct(THIS_FILE, __LINE__, __PRETTY_FUNCTION__);

 #define FuncTrace() printf(FUNC_TRACE_FORMAT, __PRETTY_FUNCTION__, THIS_FILE, __LINE__);

 #else

 #define FuncLogger()
 #define FuncTrace()

 #endif
 */

}

#endif
