#ifndef _LIBBASE_PVT_MEMORY_HPP_
#define _LIBBASE_PVT_MEMORY_HPP_

#include <windows.h>

#include <type_traits>

namespace Memory {
#ifdef MEMORY_DEBUG
	namespace Watchdog
	{
		typedef void (*pfunc)();
		extern size_t allocations;
		extern size_t deletions;
		extern uint64_t allocations_size;
		extern uint64_t deletions_size;

		extern pfunc on_delete;
	};

#endif

	typedef HANDLE Heap_t;

	inline Memory::Heap_t get_heap()
	{
		return ::GetProcessHeap();
	}

	template<typename Pointer>
	inline size_t size(Pointer in)
	{
		static_assert(std::is_pointer<Pointer>::value, "Pointer type is required");
		return (in) ? ::HeapSize(get_heap(), 0, (PVOID)in) : 0;
	}

	template<typename Pointer>
	inline Pointer malloc(size_t size, DWORD flags = 0/*HEAP_ZERO_MEMORY*/)
	{
		static_assert(std::is_pointer<Pointer>::value, "Pointer type is required");
#ifdef MEMORY_DEBUG
		Watchdog::allocations++;
		Watchdog::allocations_size += size;
#endif
		return static_cast<Pointer>(::HeapAlloc(get_heap(), flags, size));
	}

	template<typename Pointer>
	inline Pointer calloc(size_t count, DWORD flags = 0)
	{
		static_assert(std::is_pointer<Pointer>::value, "Pointer type is required");
		Pointer tmp_ptr = nullptr;
#ifdef MEMORY_DEBUG
		Watchdog::allocations++;
		Watchdog::allocations_size += sizeof(*tmp_ptr) * count;
#endif
		return static_cast<Pointer>(::HeapAlloc(get_heap(), flags | HEAP_ZERO_MEMORY, sizeof(*tmp_ptr) * count));
	}

	template<typename Pointer>
	inline Pointer ealloc(DWORD flags = 0)
	{
		static_assert(std::is_pointer<Pointer>::value, "Pointer type is required");
		Pointer tmp_ptr = nullptr;
#ifdef MEMORY_DEBUG
		Watchdog::allocations++;
		Watchdog::allocations_size += sizeof(*tmp_ptr);
#endif
		return static_cast<Pointer>(::HeapAlloc(get_heap(), flags | HEAP_ZERO_MEMORY, sizeof(*tmp_ptr)));
	}

	template<typename Pointer>
	inline void free(Pointer & in)
	{
		static_assert(std::is_pointer<Pointer>::value, "Pointer type is required");
#ifdef MEMORY_DEBUG
		Watchdog::deletions++;
		Watchdog::deletions_size += Memory::size(in);
		if (Watchdog::deletions_size == Watchdog::allocations_size && Watchdog::deletions == Watchdog::allocations)
			printf("There is no leaks\n");
#endif
		::HeapFree(get_heap(), 0, *(PVOID*)(&in));
		*(PVOID*)(&in) = nullptr;
	}

	template<typename Pointer>
	inline bool realloc(Pointer & in, size_t size, DWORD flags = HEAP_ZERO_MEMORY)
	{
#ifdef MEMORY_DEBUG
		if (in) {
			Watchdog::deletions++;
			Watchdog::deletions_size += Memory::size(in);
		}
		Watchdog::allocations++;
		Watchdog::allocations_size += size;
#endif
		static_assert(std::is_pointer<Pointer>::value, "Pointer type is required");
		return (in = static_cast<Pointer>((in) ? ::HeapReAlloc(get_heap(), flags, (PVOID)in, size) : ::HeapAlloc(get_heap(), flags, size)));
	}

	template<typename Pointer1, typename Pointer2>
	inline bool compare(Pointer1 m1, Pointer2 m2, size_t size)
	{
		static_assert(std::is_pointer<Pointer1>::value, "Pointer type is required");
		static_assert(std::is_pointer<Pointer2>::value, "Pointer type is required");
		return ::memcmp((PVOID)m1, (PVOID)m2, size) == 0;
	}

	template<typename Pointer1, typename Pointer2>
	inline Pointer1 copy(Pointer1 dest, Pointer2 src, size_t size)
	{
		static_assert(std::is_pointer<Pointer1>::value, "Pointer type is required");
		static_assert(std::is_pointer<Pointer2>::value, "Pointer type is required");
		//return ::memcpy_s(dest, sour, size);
		return static_cast<Pointer1>(::memcpy((PVOID)dest, (PVOID)src, size));
	}

	template<typename Pointer1, typename Pointer2>
	inline Pointer1 dup(Pointer2 src, size_t size, DWORD flags = 0/*HEAP_ZERO_MEMORY*/)
	{
		static_assert(std::is_pointer<Pointer1>::value, "Pointer type is required");
		static_assert(std::is_pointer<Pointer2>::value, "Pointer type is required");
		return Memory::copy(Memory::malloc<Pointer1>(size, flags), src, size);
	}

	template<typename Pointer>
	inline Pointer fill(Pointer in, size_t size, int fill = 0)
	{
		static_assert(std::is_pointer<Pointer>::value, "Pointer type is required");
		return static_cast<Pointer>(::memset((PVOID)in, fill, size));
	}

	template<typename Pointer>
	inline Pointer zero(Pointer in, size_t size)
	{
		static_assert(std::is_pointer<Pointer>::value, "Pointer type is required");
		return static_cast<Pointer>(::memset((PVOID)in, 0, size));
	}

	template<typename NotPointer>
	inline void zero(NotPointer & in)
	{
		static_assert(!std::is_pointer<NotPointer>::value, "Nonpointer type is required");
		::memset((PVOID)&in, 0, sizeof(in));
	}
}

#if defined(NoStdNew) || defined(NO_STD_NEW)
inline void * operator new(size_t size) noexcept
{
	return Memory::malloc<PVOID>(size, HEAP_ZERO_MEMORY);
}

inline void * operator new [](size_t size) noexcept
{
	return ::operator new(size);
}

inline void operator delete(void * in) noexcept
{
	Memory::free(in);
}

inline void operator delete [](void * ptr) noexcept
{
	::operator delete(ptr);
}
#endif

#endif
