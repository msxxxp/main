#ifndef BASIS_CONFIGURE_COMPILER_VC_PROLOG_HPP_
#define BASIS_CONFIGURE_COMPILER_VC_PROLOG_HPP_

//#pragma warning( disable : 4507 34; once : 4385; error : 164 )
//#pragma warning(disable: 4221)
#pragma warning(disable: 4244) // warning C4244: 'argument' : conversion from 'sync::Timeout_t' to 'DWORD', possible loss of data
#pragma warning(disable: 4267) // warning C4267: 'argument' : conversion from 'size_t' to 'int', possible loss of data
#pragma warning(disable: 4291) // warning C4291: 'void *operator new(size_t,void *,const simstd::nothrow_t &) throw()' : no matching operator delete found; memory will not be freed if initialization throws an exception
#pragma warning(disable: 4425) // warning C4425: 'unknown-type simstd::less<void>::operator ()(T &&,U &&) const' : 'constexpr' was ignored (class literal types are not yet supported)
#pragma warning(disable: 4800) // warning C4800: 'BOOL' : forcing value to bool 'true' or 'false' (performance warning)
//#pragma warning(disable: 4996)

// temp
//#pragma warning(disable: 4291)

#define _CRT_SECURE_NO_WARNINGS

#define __PRETTY_FUNCTION__ __FUNCSIG__

#endif
