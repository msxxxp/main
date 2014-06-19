#ifndef BASIS_SYS_CONFIGURE_OS_WINDOWS_HPP_
#define BASIS_SYS_CONFIGURE_OS_WINDOWS_HPP_

#if (_WIN32_WINNT < 0x0600)
#undef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif

#if (WINVER < 0x0600)
#undef WINVER
#define WINVER 0x0600
#endif

#if (_WIN32_IE < 0x0600)
#undef _WIN32_IE
#define _WIN32_IE 0x0600
#endif

//#define WIN32_LEAN_AND_MEAN
#define NOMCX
#define NOIME
//#define NOMINMAX

#include <windows.h>

const wchar_t * const PATH_SEPARATOR = L"\\"; // Path separator in the file system
const wchar_t * const PATH_SEPARATORS = L"\\/";
const wchar_t * const PATH_PREFIX_NT = L"\\\\?\\"; // Prefix to put ahead of a long path for Windows API
const wchar_t * const REPARSE_PREFIX = L"\\??\\";
const wchar_t * const MASK_CHARS = L"*?";

const wchar_t PATH_SEPARATOR_C = L'\\';

inline void mbox(const char * text, const char * capt = "")
{
	::MessageBoxA(nullptr, text, capt, MB_OK);
}

inline void mbox(const wchar_t * text, const wchar_t * capt = L"")
{
	::MessageBoxW(nullptr, text, capt, MB_OK);
}

#endif
