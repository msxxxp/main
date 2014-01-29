#ifndef LIBSTL_SYSTEM_CONFIGURE_
#define LIBSTL_SYSTEM_CONFIGURE_

#ifdef _MSC_VER
#	define _CRT_SECURE_NO_WARNINGS
//#pragma warning( disable : 4507 34; once : 4385; error : 164 )
#pragma warning(disable: 4800)
#pragma warning(disable: 4221)
#pragma warning(disable: 4244)
#pragma warning(disable: 4267)
#else
#endif

#include <stdint.h>
#include <system/sub_configure/os/windows.hpp>
#ifdef _MSC_VER
#	include <system/sub_configure/compiler/vc.hpp>
#else
#	include <system/sub_configure/compiler/gcc.hpp>
#endif
#include <system/sub_configure/obligatory/array.hpp>
#include <system/sub_configure/obligatory/constants.hpp>
#include <system/sub_configure/obligatory/definitions.hpp>
#include <system/sub_configure/obligatory/make_ll.hpp>
#include <system/sub_configure/obligatory/safe.hpp>
#include <system/sub_configure/obligatory/this_file.hpp>
#include <system/sub_configure/obligatory/Va_list.hpp>

#endif
