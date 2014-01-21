#ifndef LIBBASE_CONSOLE_HPP_
#define LIBBASE_CONSOLE_HPP_

#include <libbase/std.hpp>
#include <libbase/cstr.hpp>

namespace Base {

	size_t fileout(PCSTR str, size_t len, HANDLE hndl);

	size_t fileout(PCWSTR str, size_t len, HANDLE hndl);

	size_t fileout(wchar_t ch, size_t len, HANDLE hndl);

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
