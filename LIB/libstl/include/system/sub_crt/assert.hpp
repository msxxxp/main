#ifndef LIBSTL_SYSTEM_CRT_ASSERT_HPP_
#define LIBSTL_SYSTEM_CRT_ASSERT_HPP_

#include <system/crt.hpp>

namespace crt {

	void assrt(const char* message, const char* file, int line);

	void assrt(const wchar_t* message, const char* file, int line);

}

#ifdef NDEBUG
#	define assert(_Expression) ((void)0)
#else
#	define assert(_Expression) \
           (void) \
           ((!!(_Expression)) || \
           (crt::assrt(#_Expression, __FILE__, __LINE__), 0))
#endif

#endif
