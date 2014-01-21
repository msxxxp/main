#ifndef LIBSTL_SYSTEM_CONFIGURE_OBLIGATORY_SAFE_HPP_
#define LIBSTL_SYSTEM_CONFIGURE_OBLIGATORY_SAFE_HPP_

bool safe_vsnprintf(wchar_t * buf, size_t len, const wchar_t * format, va_list vl);

bool safe_snprintf(wchar_t * buff, size_t len, const wchar_t * format, ...);

bool safe_vsnprintf(char * buf, size_t len, const char * format, va_list vl);

bool safe_snprintf(char * buff, size_t len, const char * format, ...);

#endif
