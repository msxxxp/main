#ifndef BASIS_SYS_CONSOLE_WPRINTF_HPP_
#define BASIS_SYS_CONSOLE_WPRINTF_HPP_

#include <basis/sys/console.hpp>

namespace console {

	size_t fputs(const wchar_t * str, size_t len, HANDLE hndl);

	size_t fputs(const wchar_t * str, HANDLE hndl);

	size_t puts(const wchar_t * str, size_t len, Handle hnd = Handle::OUTPUT);

	size_t puts(const wchar_t * str, Handle hnd = Handle::OUTPUT);

	size_t putc(Handle hnd, wchar_t ch);

	size_t vprintf(Handle hnd, const wchar_t * format, va_list vl);

	size_t vprintf(const wchar_t * format, va_list vl);

	size_t fvprintf(HANDLE hndl, const wchar_t * format, va_list vl);

	int printf(Handle hnd, const wchar_t * format, ...);

	int printf(const wchar_t * format, ...);

	int wprintf(const wchar_t * format, ...);

	int fprintf(HANDLE hndl, const wchar_t * format, ...);

}

#endif
