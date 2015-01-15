#ifndef BASIS_SYS_MEMORY_MEMORY_HPP_
#define BASIS_SYS_MEMORY_MEMORY_HPP_

#include <basis/sys/memory.hpp>

#include <type_traits>

namespace memory {

//	template<typename Type>
//	inline Type & reverse_bytes(Type & inout)
//	{
//		simstd::reverse((char*)&inout, ((char*)&inout) + sizeof(inout));
//		return inout;
//	}

	inline WORD & swap_bytes(WORD & inout)
	{
		inout = static_cast<WORD>(inout >> 8 | inout << 8);
		return inout;
	}

	inline DWORD & swap_words(DWORD &inout)
	{
		inout = inout >> 16 | inout << 16;
		return inout;
	}

	inline intmax_t mega_to_bytes(size_t in)
	{
		return (in != 0) ? (intmax_t)in << 20 : -1ll;
	}

	inline size_t bytes_to_mega(intmax_t in)
	{
		return static_cast<size_t>((in > 0) ? in >> 20 : 0);
	}

}

namespace memory {

	typedef HANDLE Heap_t;

	template<typename Pointer>
	inline size_t size(Pointer ptr)
	{
		static_assert(std::is_pointer<Pointer>::value, "Pointer type is required");
		return (ptr) ? heap::DefaultStat::size(ptr) : 0;
	}

	template<typename Pointer>
	inline Pointer malloc(size_t size, DWORD flags = 0/*HEAP_ZERO_MEMORY*/)
	{
		UNUSED(flags);
		static_assert(std::is_pointer<Pointer>::value, "Pointer type is required");
		return static_cast<Pointer>(HostAlloc(heap::DefaultStat, size));
	}

	template<typename Pointer>
	inline Pointer calloc(size_t count, DWORD flags = 0)
	{
		UNUSED(flags);
		static_assert(std::is_pointer<Pointer>::value, "Pointer type is required");
		Pointer tmp_ptr = nullptr;
		return static_cast<Pointer>(HostAlloc(heap::DefaultStat, sizeof(*tmp_ptr) * count));
	}

	template<typename Pointer>
	inline Pointer ealloc(DWORD flags = 0)
	{
		UNUSED(flags);
		static_assert(std::is_pointer<Pointer>::value, "Pointer type is required");
		Pointer tmp_ptr = nullptr;
		return static_cast<Pointer>(HostAlloc(heap::DefaultStat, sizeof(*tmp_ptr)));
	}

	template<typename Pointer>
	inline void free(Pointer & in)
	{
		static_assert(std::is_pointer<Pointer>::value, "Pointer type is required");
		HostFree(heap::DefaultStat, *(void**)(&in));
		*(void**)(&in) = nullptr;
	}

	template<typename Pointer>
	inline bool realloc(Pointer& in, size_t size, DWORD flags = HEAP_ZERO_MEMORY)
	{
		UNUSED(flags);
		static_assert(std::is_pointer<Pointer>::value, "Pointer type is required");
		return (in = static_cast<Pointer>(HostRealloc(heap::DefaultStat, (void*)in, size)));
	}

	template<typename Pointer1, typename Pointer2>
	inline bool compare(Pointer1 m1, Pointer2 m2, size_t size)
	{
		static_assert(std::is_pointer<Pointer1>::value, "Pointer type is required");
		static_assert(std::is_pointer<Pointer2>::value, "Pointer type is required");
		return ::memcmp((const void*)m1, (const void*)m2, size) == 0;
	}

	template<typename Pointer1, typename Pointer2>
	inline Pointer1 copy(Pointer1 dest, Pointer2 src, size_t size)
	{
		static_assert(std::is_pointer<Pointer1>::value, "Pointer type is required");
		static_assert(std::is_pointer<Pointer2>::value, "Pointer type is required");
		//return ::memcpy_s(dest, sour, size);
		return static_cast<Pointer1>(::memcpy((void*)dest, (void*)src, size));
	}

	template<typename Pointer1, typename Pointer2>
	inline Pointer1 move(Pointer1 dest, Pointer2 src, size_t size)
	{
		static_assert(std::is_pointer<Pointer1>::value, "Pointer type is required");
		static_assert(std::is_pointer<Pointer2>::value, "Pointer type is required");
		return static_cast<Pointer1>(::memmove((void*)dest, (void*)src, size));
	}

	template<typename Pointer1, typename Pointer2>
	inline Pointer1 dup(Pointer2 src, size_t size)
	{
		static_assert(std::is_pointer<Pointer1>::value, "Pointer type is required");
		static_assert(std::is_pointer<Pointer2>::value, "Pointer type is required");
		return memory::copy(memory::malloc<Pointer1>(size), src, size);
	}

	template<typename Pointer>
	inline Pointer fill(Pointer in, size_t bytes, int fill)
	{
		static_assert(std::is_pointer<Pointer>::value, "Pointer type is required");
		return static_cast<Pointer>(::memset((void*)in, fill, bytes));
	}

	template<typename NonPointer>
	inline void fill(NonPointer & in, int fill)
	{
		static_assert(!std::is_pointer<NonPointer>::value, "Nonpointer type is required");
		::memset((void*)&in, fill, sizeof(in));
	}

	template<typename Pointer>
	inline Pointer zero(Pointer in, size_t bytes)
	{
		static_assert(std::is_pointer<Pointer>::value, "Pointer type is required");
		return static_cast<Pointer>(::memset((void*)in, 0, bytes));
	}

	template<typename NonPointer>
	inline void zero(NonPointer & in)
	{
		static_assert(!std::is_pointer<NonPointer>::value, "Nonpointer type is required");
		::memset((void*)&in, 0, sizeof(in));
	}

}

#endif
