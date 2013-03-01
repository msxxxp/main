#ifndef _LIBBASE_STD_HPP_
#define _LIBBASE_STD_HPP_

#if (_WIN32_WINNT < 0x0600)
#undef _WIN32_WINNT
#endif

#define _WIN32_WINNT 0x0600

#if (WINVER < 0x0600)
#undef WINVER
#endif
#ifndef WINVER
#define WINVER 0x0600
#endif

#if (_WIN32_IE < 0x0600)
#undef _WIN32_IE
#endif
#ifndef _WIN32_IE
#define _WIN32_IE 0x0600
#endif

//#define WIN32_LEAN_AND_MEAN
#define NOMCX
#define NOIME
#define NOMINMAX

#include <cstdint>
#include <windows.h>
#include <stdio.h>

#include <type_traits>

#ifdef _MSC_VER
	typedef int WINBOOL;
	typedef SSIZE_T ssize_t;
#define noexcept
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

namespace Memory {
#ifdef DEBUG
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
	inline Pointer alloc(size_t size, DWORD flags = HEAP_ZERO_MEMORY)
	{
#ifdef DEBUG
		Watchdog::allocations++;
		Watchdog::allocations_size += size;
#endif
		static_assert(std::is_pointer<Pointer>::value, "Pointer type is required");
		return static_cast<Pointer>(::HeapAlloc(get_heap(), flags, size));
	}

	template<typename Pointer>
	inline void free(Pointer & in)
	{
#ifdef DEBUG
		Watchdog::deletions++;
		Watchdog::deletions_size += Memory::size(in);
		if (Watchdog::deletions_size == Watchdog::allocations_size && Watchdog::deletions == Watchdog::allocations)
			printf("There is no leaks\n");
#endif
		static_assert(std::is_pointer<Pointer>::value, "Pointer type is required");
		::HeapFree(get_heap(), 0, (PVOID)in);
		in = nullptr;
	}

	template<typename Pointer>
	inline bool realloc(Pointer & in, size_t size, DWORD flags = HEAP_ZERO_MEMORY)
	{
#ifdef DEBUG
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
	inline Pointer1 copy(Pointer1 dest, Pointer2 sour, size_t size)
	{
		static_assert(std::is_pointer<Pointer1>::value, "Pointer type is required");
		static_assert(std::is_pointer<Pointer2>::value, "Pointer type is required");
		//return ::memcpy_s(dest, sour, size);
		return static_cast<Pointer1>(::memcpy((PVOID)dest, (PVOID)sour, size));
	}

	template<typename Pointer>
	inline Pointer fill(Pointer in, size_t size, char fill = 0)
	{
		static_assert(std::is_pointer<Pointer>::value, "Pointer type is required");
		return static_cast<Pointer>(::memset((PVOID)in, (int)fill, size));
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

#ifdef NoStdNew
inline void * operator new(size_t size) noexcept
{
	return Memory::alloc<PVOID>(size, HEAP_ZERO_MEMORY);
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

#include <algorithm>

#ifdef NoStlString
template<typename Type>
struct AutoSTR;

typedef AutoSTR<char> astring;
typedef AutoSTR<wchar_t> ustring;
#else
#ifdef _MSC_VER
	#include <string>
#else
#include <iosfwd>
#endif
typedef std::string astring;
typedef std::wstring ustring;
#endif

typedef const void * PCVOID;

#define DEFINE_FUNC(name) F##name name
#define GET_DLL_FUNC(name) name = (F##name)get_function(#name)
#define GET_DLL_FUNC_NT(name) name = (F##name)get_function_nt(#name)

#define THIS_FILE Base::filename_only(__FILE__)
#define THIS_PLACE THIS_FILE, __LINE__, __PRETTY_FUNCTION__

#ifndef sizeofa
#define sizeofa(array) (sizeof(array)/sizeof(0[array]))
#endif

#ifndef sizeofe
#define sizeofe(array) (sizeof(0[array]))
#endif

namespace Base {

	PCWSTR const EMPTY_STR = L"";
	PCWSTR const SPACE = L" ";
	PCWSTR const PATH_SEPARATOR = L"\\"; // Path separator in the file system
	PCWSTR const PATH_SEPARATORS = L"\\/";
	PCWSTR const PATH_PREFIX_NT = L"\\\\?\\"; // Prefix to put ahead of a long path for Windows API
	PCWSTR const NETWORK_PATH_PREFIX = L"\\\\";

	const wchar_t PATH_SEPARATOR_C = L'\\';
	const wchar_t STR_END = L'\0';
	const wchar_t SPACE_C = L' ';

	const uint32_t BOM_UTF32le = 0x0000FEFF;
	const uint32_t BOM_UTF32be = 0xFFFE0000;
	const uint32_t BOM_UTF16le = 0xFEFF;
	const uint32_t BOM_UTF16be = 0xFFFE;
	const uint32_t BOM_UTF8 = 0xBFBBEF00;

	const size_t MAX_PATH_LEN = 32772;
	const UINT CP_UTF16le = 1200;
	const UINT CP_UTF16be = 1201;
	const UINT CP_UTF32le = 1202;
	const UINT CP_UTF32be = 1203;
//	const size_t CP_AUTODETECT = (UINT)-1;
	const UINT DEFAULT_CP = CP_UTF8;

	enum class WaitResult_t : size_t {
		SUCCESS   = WAIT_OBJECT_0,
		APC       = WAIT_IO_COMPLETION,
		TIMEOUT   = WAIT_TIMEOUT,
		FAILED    = WAIT_FAILED,
		ABANDONED = WAIT_ABANDONED,
	};

	PCWSTR to_str(WaitResult_t waitResult);

	typedef ssize_t Timeout_t;

	static const Timeout_t WAIT_FOREVER = INFINITE;

	template<typename Type, size_t N>
	size_t lengthof(Type (&)[N])
	{
		return N;
	}

	inline uint64_t make_uint64(uint32_t high, uint32_t low)
	{
		return ((uint64_t)high) << 32 | low;
	}

	inline uint32_t high_part_64(uint64_t arg64)
	{
		return (uint32_t)(arg64 >> 32);
	}

	inline uint32_t low_part_64(uint64_t arg64)
	{
		return (uint32_t)(arg64 & 0xFFFFFFFFULL);
	}

	inline PCSTR filename_only(PCSTR path, char ch = '\\')
	{
#ifdef _MSC_VER
		PCSTR tmp = strrchr((path), ch);
		return ( tmp ? tmp : (path) - 1) + 1;
#else
		return (strrchr((path), ch) ? : (path) - 1) + 1;
#endif
	}

	inline PCWSTR filename_only(PCWSTR path, wchar_t ch = PATH_SEPARATOR_C)
	{
#ifdef _MSC_VER
		PCWSTR tmp = wcsrchr((path), ch);
		return ( tmp ? tmp : (path) - 1) + 1;
#else
		return (wcsrchr((path), ch) ? : (path) - 1) + 1;
#endif
	}

	///=============================================================================================
	inline void mbox(PCSTR text, PCSTR capt = "")
	{
		::MessageBoxA(nullptr, text, capt, MB_OK);
	}

	inline void mbox(PCWSTR text, PCWSTR capt = L"")
	{
		::MessageBoxW(nullptr, text, capt, MB_OK);
	}

}

#endif
