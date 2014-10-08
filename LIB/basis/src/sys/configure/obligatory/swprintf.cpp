#include <basis/configure.hpp>

#include <wchar.h>

size_t safe_vsnprintf(wchar_t * buf, size_t len, const wchar_t * format, va_list vl)
{
	buf[--len] = 0;
	int ret = ::_vsnwprintf(buf, len, format, vl);
	return ret == -1 ? 0 : ret;
}

size_t safe_snprintf(wchar_t * buff, size_t len, const wchar_t * format, ...)
{
	Va_list vl;
	va_start(vl, format);
	return safe_vsnprintf(buff, len, format, vl);
}
