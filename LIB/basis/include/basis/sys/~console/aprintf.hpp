#ifndef BASIS_SYS_CONSOLE_APRINTF_HPP_
#define BASIS_SYS_CONSOLE_APRINTF_HPP_

namespace console {

	size_t fputs(const char* str, size_t len, HANDLE hndl);

	size_t fputs(const char* str, HANDLE hndl);

	size_t puts(const char* str, size_t len, Handle hnd = Handle::OUTPUT);

	size_t puts(const char* str, Handle hnd = Handle::OUTPUT);

	size_t putc(Handle hnd, char ch);

	size_t vprintf(Handle hnd, const char* format, va_list vl);

	size_t vprintf(const char* format, va_list vl);

	size_t fvprintf(HANDLE hndl, const char* format, va_list vl);

	int printf(Handle hnd, const char* format, ...);

	int printf(const char* format, ...);

	int aprintf(const char* format, ...);

	int fprintf(HANDLE hndl, const char* format, ...);

}

#endif
