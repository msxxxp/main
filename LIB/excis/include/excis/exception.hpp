#ifndef EXCIS_EXCEPTION_HPP_
#define EXCIS_EXCEPTION_HPP_

#include <basis/configure.hpp>

#include <basis/sys/cstr.hpp>

#include <basis/simstd/iosfwd>
#include <basis/simstd/memory>
//#include <basis/simstd/string>

#include <excis/~exception/Abstract.hpp>
#include <excis/~exception/SEH.hpp>

#ifdef NDEBUG
#include <excis/~exception/exception_ndebug.hpp>
#else
#include <excis/~exception/exception_debug.hpp>
#endif

#endif
