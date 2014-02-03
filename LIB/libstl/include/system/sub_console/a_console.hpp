#ifndef LIBBASE_CONSOLE_A_CONSOLE_HPP_
#define LIBBASE_CONSOLE_A_CONSOLE_HPP_

#include <system/console.hpp>

namespace console {

	size_t putc(Handle hnd, char ch);

	size_t puts(Handle hnd, const char * str, size_t len);

	size_t puts(const char * str, size_t len, Handle hnd = Handle::OUTPUT);

	size_t puts(const char * str, Handle hnd = Handle::OUTPUT);

	size_t vprintf(Handle hnd, const char * format, va_list vl);

	size_t vprintf(const char * format, va_list vl);

	int printf(Handle hnd, const char * format, ...);

	int printf(const char * format, ...);

}

#endif
