#ifndef LIBBASE_CONSOLE_W_CONSOLE_HPP_
#define LIBBASE_CONSOLE_W_CONSOLE_HPP_

#include <system/console.hpp>

namespace console {

	size_t putc(Handle hnd, wchar_t ch);

	size_t puts(Handle hnd, const wchar_t * str, size_t len);

	size_t puts(const wchar_t * str, size_t len, Handle hnd = Handle::OUTPUT);

	size_t puts(const wchar_t * str, Handle hnd = Handle::OUTPUT);

	size_t vprintf(Handle hnd, const wchar_t * format, va_list vl);

	size_t vprintf(const wchar_t * format, va_list vl);

	int printf(Handle hnd, const wchar_t * format, ...);

	int printf(const wchar_t * format, ...);

	int wprintf(const wchar_t * format, ...);

}

#endif
