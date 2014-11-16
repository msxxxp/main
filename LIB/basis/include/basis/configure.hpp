#ifndef BASIS_CONFIGURE_HPP_
#define BASIS_CONFIGURE_HPP_

#include <basis/sys/~configure/os/windows_prolog.hpp>

#ifdef _MSC_VER
#	include <basis/sys/~configure/compiler/vc_prolog.hpp>
#else
//#	include <basis/sys/~configure/compiler/gcc.hpp>
#endif

#include <basis/sys/~configure/os/windows_epilog.hpp>

#ifdef _MSC_VER
#	include <basis/sys/~configure/compiler/vc_epilog.hpp>
#else
#	include <basis/sys/~configure/compiler/gcc.hpp>
#endif

#include <basis/types.hpp>

#include <basis/sys/~configure/obligatory/array.hpp>
#include <basis/sys/~configure/obligatory/constants.hpp>
#include <basis/sys/~configure/obligatory/definitions.hpp>
#include <basis/sys/~configure/obligatory/make_ll.hpp>
#include <basis/sys/~configure/obligatory/safe.hpp>
#include <basis/sys/~configure/obligatory/this_file.hpp>
#include <basis/sys/~configure/obligatory/types.hpp>
#include <basis/sys/~configure/obligatory/Va_list.hpp>

#endif
