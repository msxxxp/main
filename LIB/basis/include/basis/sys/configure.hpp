#ifndef BASIS_SYS_CONFIGURE_
#define BASIS_SYS_CONFIGURE_

#ifdef _MSC_VER
#	define _CRT_SECURE_NO_WARNINGS
//#pragma warning( disable : 4507 34; once : 4385; error : 164 )
#pragma warning(disable: 4800)
#pragma warning(disable: 4221)
#pragma warning(disable: 4244)
#pragma warning(disable: 4267)

// temp
#pragma warning(disable: 4291)
#else
#endif

#include <basis/sys/base_types.hpp>

#include <basis/sys/sub_configure/os/windows.hpp>

#ifdef _MSC_VER
#	include <basis/sys/sub_configure/compiler/vc.hpp>
#else
#	include <basis/sys/sub_configure/compiler/gcc.hpp>
#endif

#include <basis/sys/sub_configure/obligatory/array.hpp>
#include <basis/sys/sub_configure/obligatory/constants.hpp>
#include <basis/sys/sub_configure/obligatory/definitions.hpp>
#include <basis/sys/sub_configure/obligatory/make_ll.hpp>
#include <basis/sys/sub_configure/obligatory/safe.hpp>
#include <basis/sys/sub_configure/obligatory/this_file.hpp>
#include <basis/sys/sub_configure/obligatory/types.hpp>
#include <basis/sys/sub_configure/obligatory/Va_list.hpp>

#endif
