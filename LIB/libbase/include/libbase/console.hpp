#ifndef LIBBASE_CONSOLE_HPP_
#define LIBBASE_CONSOLE_HPP_

#include <libbase/std.hpp>
#include <libbase/cstr.hpp>

namespace Base {
	namespace Console {
		enum class Handle : size_t {
			INPUT  = STD_INPUT_HANDLE,
			OUTPUT = STD_OUTPUT_HANDLE,
			ERR    = STD_ERROR_HANDLE,
		};

		size_t out(Handle hnd, PCWSTR str, size_t len);

		size_t vprintf(Handle hnd, PCWSTR format, va_list vl);

		int printf(Handle hnd, PCWSTR format, ...);

		int printf(PCWSTR format, ...);

		inline size_t out(PCWSTR str, size_t len, Handle hnd = Handle::OUTPUT)
		{
			return out(hnd, str, len);
		}

		inline size_t out(PCWSTR str, Handle hnd = Handle::OUTPUT)
		{
			return out(hnd, str, Cstr::length(str));
		}

		size_t out(wchar_t ch, Handle hnd = Handle::OUTPUT);

		size_t out(PCSTR str, size_t len, Handle hnd = Handle::OUTPUT);

		inline size_t vprintf(PCWSTR format, va_list vl)
		{
			return vprintf(Handle::OUTPUT, format, vl);
		}

		///=================================================================================== Color
		struct Color {
			~Color();

			Color(WORD color);

			void restore();

		private:
			bool save();

			WORD m_color;
		};
	}

	size_t fileout(PCSTR str, size_t len, HANDLE hndl);

	size_t fileout(PCWSTR str, size_t len, HANDLE hndl);

	size_t fileout(wchar_t ch, size_t len, HANDLE hndl);

	///================================================================================= Console out

	void errx(int eval, PCSTR format, ...);

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
