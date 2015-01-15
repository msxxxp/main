#ifndef BASIS_SIMSTD_MEMORY_NEW_HPP_
#define BASIS_SIMSTD_MEMORY_NEW_HPP_

#include <basis/sys/memory.hpp>

namespace simstd {
	struct nothrow_t
	{
	};

#if defined(__GNUC__) && (__GNUC__ < 3)
	extern nothrow_t nothrow;
#else
	const nothrow_t nothrow;
#endif
}

///=========================================================================== no exception versions
inline void* operator new(size_t size, const simstd::nothrow_t&) throw()
{
	return HostAlloc(memory::heap::DefaultStat, size);
}

inline void* operator new[](size_t size, const simstd::nothrow_t&) throw()
{
	return HostAlloc(memory::heap::DefaultStat, size);
}

inline void operator delete(void* ptr, const simstd::nothrow_t&) throw()
{
	HostFree(memory::heap::DefaultStat, ptr);
}

inline void operator delete[](void * ptr, const simstd::nothrow_t&) throw()
{
	HostFree(memory::heap::DefaultStat, ptr);
}

///===================================================================== Default no exception global
#ifdef NO_STD_NEW
inline void* operator new(size_t size)
{
	return operator new(size, simstd::nothrow);
}

inline void* operator new[](size_t size)
{
	return operator new[](size, simstd::nothrow);
}

inline void operator delete(void* ptr)
{
	operator delete(ptr, simstd::nothrow);
}

inline void operator delete[](void * ptr)
{
	operator delete[](ptr, simstd::nothrow);
}
#endif

///=============================================================================== Default placement
//inline void* operator new(size_t, void* p) throw()
//{
//	return p;
//}
//
//inline void* operator new[](size_t, void* p) throw()
//{
//	return p;
//}
//
//inline void operator delete(void*, void*) throw()
//{
//}
//
//inline void operator delete [](void*, void*) throw()
//{
//}

///=============================================================================== Special placement
inline void* operator new(size_t, void* p, const simstd::nothrow_t&) throw()
{
	return p;
}

inline void* operator new[](size_t, void* p, const simstd::nothrow_t&) throw()
{
	return p;
}

#endif
