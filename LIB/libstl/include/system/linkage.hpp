#ifndef LIBSTL_SYSTEM_LINKAGE_
#define LIBSTL_SYSTEM_LINKAGE_

#include <system/configure.hpp>
#include <extra/pattern.hpp>

#define DEFINE_FUNC(name)     F##name name
#define GET_DLL_FUNC(name)    name = (F##name)get_function(#name)
#define GET_DLL_FUNC_NT(name) name = (F##name)get_function_nt(#name)

#include <system/sub_linkage/dll.hpp>

#endif
