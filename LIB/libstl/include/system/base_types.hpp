#ifndef LIBSTL_SYSTEM_BASE_TYPES_
#define LIBSTL_SYSTEM_BASE_TYPES_

#include <stdint.h>

namespace simstd {
	typedef decltype(nullptr) nullptr_t;
}

//#ifdef __x86_64__
//#define nullptr 0LL
//#else
//#define nullptr 0
//#endif

namespace simstd {
//#ifdef __x86_64__
//	typedef long long unsigned int size_t;
//	typedef long long int ssize_t;
//	typedef long long int ptrdiff_t;
//#else
//	typedef unsigned int size_t;
//	typedef int ssize_t;
//	typedef int ptrdiff_t;
//#endif

	namespace Test {
		typedef int (*aPrintFunc)(const char *, ...);
		typedef int (*wPrintFunc)(const wchar_t *, ...);
	}
}

#endif
