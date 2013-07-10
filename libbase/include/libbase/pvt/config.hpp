#ifndef _LIBBASE_PVT_CONFIG_HPP_
#define _LIBBASE_PVT_CONFIG_HPP_

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
#define NOMINMAX

#include <cstdint>
#include <windows.h>

#ifdef _MSC_VER
	typedef int WINBOOL;
	typedef SSIZE_T ssize_t;
#define noexcept
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

#endif
