#include <basis/configure.hpp>

#include <stdio.h>

size_t safe_vsnprintf(char* buf, size_t len, const char* format, va_list vl)
{
	buf[--len] = 0;
	int ret = ::_vsnprintf(buf, len, format, vl);
	return ret == -1 ? 0 : ret;
}

size_t safe_snprintf(char* buff, size_t len, const char* format, ...)
{
	Va_list vl;
	va_start(vl, format);
	return safe_vsnprintf(buff, len, format, vl);
}
