#ifndef BASIS_CONFIGURE_OBLIGATORY_SAFE_HPP_
#define BASIS_CONFIGURE_OBLIGATORY_SAFE_HPP_

size_t safe_vsnprintf(wchar_t* buf, size_t len, const wchar_t* format, va_list vl);

size_t safe_snprintf(wchar_t* buff, size_t len, const wchar_t* format, ...);

size_t safe_vsnprintf(char* buf, size_t len, const char* format, va_list vl);

size_t safe_snprintf(char* buff, size_t len, const char* format, ...);

#endif
