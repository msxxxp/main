#ifndef BASIS_SYS_CRT_ASSERT_HPP_
#define BASIS_SYS_CRT_ASSERT_HPP_

namespace crt {

	void assrt(const char* message, const char* file, int line);

	void assrt(const wchar_t* message, const char* file, int line);

}

#ifdef NDEBUG
#	define CRT_ASSERT(_Expression_) ((void)0)
#else
#	define CRT_ASSERT(_Expression_) \
           (void) \
           ((!!(_Expression_)) || \
           (crt::assrt(#_Expression_, __FILE__, __LINE__), 0))
#endif

#endif
