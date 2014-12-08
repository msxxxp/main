#ifndef BASIS_SYS_LINKAGE_
#define BASIS_SYS_LINKAGE_

#include <basis/configure.hpp>
#include <basis/ext/pattern.hpp>

#define DEFINE_FUNC(name)     F##name name;
#define GET_DLL_FUNC(name)    name = (F##name)get_function(#name);
#define GET_DLL_FUNC_NT(name) name = (F##name)get_function_nt(#name);

#include <basis/sys/~linkage/APIHook.h>
#include <basis/sys/~linkage/dll.hpp>

#endif
