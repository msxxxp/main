#ifndef LIBSTL_SYSTEM_CONSOLE_
#define LIBSTL_SYSTEM_CONSOLE_

#include <system/configure.hpp>
#include <system/cstr.hpp>
#include <system/sub_console/Color.hpp>

namespace console {

	enum class Handle : uint32_t {
		INPUT  = STD_INPUT_HANDLE,
		OUTPUT = STD_OUTPUT_HANDLE,
		ERR    = STD_ERROR_HANDLE,
	};

	size_t puts(Handle hnd, const wchar_t * str, size_t len);

	size_t puts(Handle hnd, const char * str, size_t len);


	size_t vprintf(Handle hnd, const wchar_t * format, va_list vl);

	size_t vprintf(Handle hnd, const char * format, va_list vl);


	int printf(Handle hnd, const wchar_t * format, ...);

	int printf(Handle hnd, const char * format, ...);


	inline size_t puts(const wchar_t * str, size_t len, Handle hnd = Handle::OUTPUT)
	{
		return puts(hnd, str, len);
	}

	inline size_t puts(const wchar_t * str, Handle hnd = Handle::OUTPUT)
	{
		return puts(hnd, str, Cstr::length(str));
	}


	inline size_t puts(const char * str, size_t len, Handle hnd = Handle::OUTPUT)
	{
		return puts(hnd, str, len);
	}

	inline size_t puts(const char * str, Handle hnd = Handle::OUTPUT)
	{
		return puts(hnd, str, Cstr::length(str));
	}


	inline size_t vprintf(const wchar_t * format, va_list vl)
	{
		return vprintf(Handle::OUTPUT, format, vl);
	}

	inline size_t vprintf(const char * format, va_list vl)
	{
		return vprintf(Handle::OUTPUT, format, vl);
	}


	int printf(const wchar_t * format, ...);

	int printf(const char * format, ...);

}

#endif
