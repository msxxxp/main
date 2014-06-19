#include <basis/configure.hpp>

#include <wchar.h>

bool safe_vsnprintf(wchar_t * buf, size_t len, const wchar_t * format, va_list vl)
{
	buf[--len] = 0;
	return static_cast<unsigned>(::_vsnwprintf(buf, len, format, vl)) <= len;
}

bool safe_snprintf(wchar_t * buff, size_t len, const wchar_t * format, ...)
{
	Va_list vl;
	va_start(vl, format);
	return safe_vsnprintf(buff, len, format, vl);
}
