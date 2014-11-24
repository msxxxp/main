#ifndef BASIS_SYS_CONSOLE_CONSOLE_HPP_
#define BASIS_SYS_CONSOLE_CONSOLE_HPP_

#include <basis/sys/console.hpp>

namespace console {

	enum class Handle : uint32_t {
		INPUT  = STD_INPUT_HANDLE,
		OUTPUT = STD_OUTPUT_HANDLE,
		ERR    = STD_ERROR_HANDLE,
	};

	enum class Codepage: size_t {
		UTF16LE = 1200,
		UTF16BE = 1201,
		UTF8 = 65001,
	};

	size_t get_output_codepage();

	void set_output_codepage(ssize_t cp);

	void set_output_codepage(Codepage cp);
}

/*
 #ifndef NDEBUG
 extern const char * FUNC_ENTER_FORMAT;
 extern const char * FUNC_LEAVE_FORMAT;
 extern const char * FUNC_TRACE_FORMAT;

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

#endif
