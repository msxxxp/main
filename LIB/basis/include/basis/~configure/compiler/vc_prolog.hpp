#ifndef BASIS_CONFIGURE_COMPILER_VC_PROLOG_HPP_
#define BASIS_CONFIGURE_COMPILER_VC_PROLOG_HPP_

//#pragma warning( disable : 4507 34; once : 4385; error : 164 )
//#pragma warning(disable: 4221)
//#pragma warning(disable: 4244)
//#pragma warning(disable: 4267)
#pragma warning(disable: 4425) // warning C4425: 'unknown-type simstd::less<void>::operator ()(T &&,U &&) const' : 'constexpr' was ignored (class literal types are not yet supported)
#pragma warning(disable: 4800) // warning C4800: 'BOOL' : forcing value to bool 'true' or 'false' (performance warning)
//#pragma warning(disable: 4996)

// temp
//#pragma warning(disable: 4291)

#define _CRT_SECURE_NO_WARNINGS

#define __PRETTY_FUNCTION__ __FUNCSIG__

#endif
