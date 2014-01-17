#ifndef LIBSTL_TYPES_HPP_
#define LIBSTL_TYPES_HPP_

#ifdef __x86_64__
#define nullptr 0LL
#else
#define nullptr 0
#endif

namespace simstd {
#ifdef __x86_64__
	typedef long long unsigned int size_t;
	typedef long long int ssize_t;
	typedef long long int ptrdiff_t;
#else
	typedef unsigned int size_t;
	typedef int ssize_t;
	typedef int ptrdiff_t;
#endif
}

#endif
