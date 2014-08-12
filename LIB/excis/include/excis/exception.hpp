#ifndef _LIBEXT_EXCEPTION_HPP_
#define _LIBEXT_EXCEPTION_HPP_

#include <basis/configure.hpp>
#include <basis/sys/cstr.hpp>

#include <basis/std/iosfwd>
#include <basis/std/memory>

#include <excis/sub_exception/AbstractError.hpp>
#include <excis/sub_exception/SEH.hpp>

#ifdef NDEBUG
#include <excis/sub_exception/exception_ndebug.hpp>
#else
#include <excis/sub_exception/exception_debug.hpp>
#endif

#endif
