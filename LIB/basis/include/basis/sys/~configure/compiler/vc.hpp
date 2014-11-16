#ifndef BASIS_SYS_CONFIGURE_COMPILER_VC_HPP_
#define BASIS_SYS_CONFIGURE_COMPILER_VC_HPP_

#define _CRT_SECURE_NO_WARNINGS

//#pragma warning( disable : 4507 34; once : 4385; error : 164 )
#pragma warning(disable: 4800)
#pragma warning(disable: 4221)
#pragma warning(disable: 4244)
#pragma warning(disable: 4267)
#pragma warning(disable: 4425)

// temp
#pragma warning(disable: 4291)

typedef int WINBOOL;
typedef SSIZE_T ssize_t;
//#define constexpr const
//#define noexcept
#define __PRETTY_FUNCTION__ __FUNCSIG__

#endif
