#ifndef BASIS_CONFIGURE_OS_WINDOWS_EPILOG_HPP_
#define BASIS_CONFIGURE_OS_WINDOWS_EPILOG_HPP_

#include <windows.h>

const wchar_t* const PATH_SEPARATOR = L"\\"; // Path separator in the file system
const wchar_t* const PATH_SEPARATORS = L"\\/";
const wchar_t* const PATH_PREFIX_NT = L"\\\\?\\"; // Prefix to put ahead of a long path for Windows API
const wchar_t* const REPARSE_PREFIX = L"\\??\\";
const wchar_t* const MASK_CHARS = L"*?";

const wchar_t PATH_SEPARATOR_C = L'\\';

inline void mbox(const char* text, const char* capt = "")
{
	::MessageBoxA(nullptr, text, capt, MB_OK);
}

inline void mbox(const wchar_t* text, const wchar_t* capt = L"")
{
	::MessageBoxW(nullptr, text, capt, MB_OK);
}

#endif
