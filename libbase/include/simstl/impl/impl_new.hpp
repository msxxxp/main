#ifndef SARALIB_STL_NEW_HPP_
#define SARALIB_STL_NEW_HPP_

#include "impl_types.hpp"

namespace sarastd {
	struct nothrow_t {
	};

	const nothrow_t nothrow;
}

extern "C" {
	void * malloc(sarastd::size_t);
	void free(void*);
}

inline void* operator new(sarastd::size_t size)
{
	return malloc(size);
}

inline void* operator new[](sarastd::size_t size)
{
	return ::operator new(size);
}

inline void operator delete(void* ptr)
{
	free(ptr);
}

inline void operator delete[](void * ptr)
{
	::operator delete(ptr);
}




inline void* operator new(sarastd::size_t size, const sarastd::nothrow_t&) throw()
{
	return malloc(size);
}

inline void* operator new[](sarastd::size_t size, const sarastd::nothrow_t& tag) throw()
{
	return ::operator new(size, tag);
}

inline void operator delete(void* ptr, const sarastd::nothrow_t&) throw()
{
	free(ptr);
}

inline void operator delete[](void * ptr, const sarastd::nothrow_t& tag) throw()
{
	::operator delete(ptr, tag);
}

// Default placement versions of operator new.
inline void* operator new(sarastd::size_t, void* p) throw()
{
	return p;
}

inline void* operator new[](sarastd::size_t, void* p) throw()
{
	return p;
}

// Default placement versions of operator delete.
inline void operator delete(void*, void*) throw()
{
}

inline void operator delete [](void*, void*) throw()
{
}

#endif
