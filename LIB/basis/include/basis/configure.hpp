#ifndef BASIS_CONFIGURE_HPP_
#define BASIS_CONFIGURE_HPP_

#include <basis/~configure/defines.hpp>

#include <basis/~configure/os/windows_prolog.hpp>

#ifdef _MSC_VER
#	include <basis/~configure/compiler/vc_prolog.hpp>
#else
//#	include <basis/~configure/compiler/gcc.hpp>
#endif

#include <basis/~configure/os/windows_epilog.hpp>

#ifdef _MSC_VER
#	include <basis/~configure/compiler/vc_epilog.hpp>
#else
#	include <basis/~configure/compiler/gcc.hpp>
#endif

#include <basis/~configure/obligatory/types.hpp>
#include <basis/~configure/simstd/types.hpp>
#include <basis/~configure/simstd/algorithm.hpp>
#include <basis/~configure/obligatory/array.hpp>
#include <basis/~configure/obligatory/constants.hpp>
#include <basis/~configure/obligatory/definitions.hpp>
#include <basis/~configure/obligatory/make_ll.hpp>
#include <basis/~configure/obligatory/safe.hpp>
#include <basis/~configure/obligatory/this_file.hpp>
#include <basis/~configure/obligatory/Va_list.hpp>
#include <basis/~configure/simstd/rel_ops.hpp>
#include <basis/~configure/printf_trace.hpp>

#endif
