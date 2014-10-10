#ifndef _LIBEXT_EXCEPTION_HPP_
#define _LIBEXT_EXCEPTION_HPP_

#include <basis/configure.hpp>
#include <basis/sys/cstr.hpp>

#include <basis/simstd/iosfwd>
#include <basis/simstd/memory>
#include <basis/simstd/string>

#include <excis/sub_exception/AbstractError.hpp>
#include <excis/sub_exception/SEH.hpp>

#ifdef NDEBUG
#include <excis/sub_exception/exception_ndebug.hpp>
#else
#include <excis/sub_exception/exception_debug.hpp>
#endif

#endif
