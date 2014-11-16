#ifndef BASIS_CONFIGURE_OS_WINDOWS_PROLOG_HPP_
#define BASIS_CONFIGURE_OS_WINDOWS_PROLOG_HPP_

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

#endif
