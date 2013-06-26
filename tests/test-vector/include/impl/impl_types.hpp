#ifndef SARALIB_STL_TYPES_HPP_
#define SARALIB_STL_TYPES_HPP_

namespace sarastd
{
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
