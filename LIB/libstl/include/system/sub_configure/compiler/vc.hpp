#ifndef LIBSTL_SYSTEM_CONFIGURE_COMPILER_VC_HPP_
#define LIBSTL_SYSTEM_CONFIGURE_COMPILER_VC_HPP_

#ifdef _MSC_VER
	typedef int WINBOOL;
	typedef SSIZE_T ssize_t;
//#define constexpr const
//#define noexcept
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

#endif
