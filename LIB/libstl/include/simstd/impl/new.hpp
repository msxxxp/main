#ifndef LIBSTL_NEW_HPP_
#define LIBSTL_NEW_HPP_

#include <system/base_types.hpp>

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

extern "C" {
	void * _system_malloc(size_t size);

	void _system_free(void * ptr);

	void * _system_movable_malloc(size_t size);

	void _system_movable_free(void * handle);

	void * _system_movable_lock(void * handle);

	void _system_movable_unlock(void * handle);
}

///=========================================================================== no exception versions
inline void* operator new(size_t size, const simstd::nothrow_t&) throw()
{
	return _system_malloc(size);
}

inline void* operator new[](size_t size, const simstd::nothrow_t&) throw()
{
	return _system_malloc(size);
}

inline void operator delete(void* ptr, const simstd::nothrow_t&) throw()
{
	_system_free(ptr);
}

inline void operator delete[](void * ptr, const simstd::nothrow_t&) throw()
{
	_system_free(ptr);
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
