#include <system/configure.hpp>

#include <stdio.h>

bool safe_vsnprintf(char * buf, size_t len, const char * format, va_list vl)
{
	buf[--len] = 0;
	return static_cast<unsigned>(::_vsnprintf(buf, len, format, vl)) <= len;
}

bool safe_snprintf(char * buff, size_t len, const char * format, ...)
{
	Va_list vl;
	va_start(vl, format);
	return safe_vsnprintf(buff, len, format, vl);
}
