#ifndef LIBBASE_STD_HPP_
#define LIBBASE_STD_HPP_

#include "pvt/config.hpp"
#include "pvt/memory.hpp"
#include "pvt/fwd.hpp"

#define DEFINE_FUNC(name)     F##name name
#define GET_DLL_FUNC(name)    name = (F##name)get_function(#name)
#define GET_DLL_FUNC_NT(name) name = (F##name)get_function_nt(#name)

#define UNUSED(val)      ((void)val)
#define THIS_FILE        Base::filename_only(__FILE__)
#define THIS_PLACE       THIS_FILE, __LINE__, __PRETTY_FUNCTION__
#define THIS_PLACE_SHORT THIS_FILE, __LINE__, __FUNCTION__

typedef const void * PCVOID;

namespace Base {

	PCWSTR const EMPTY_STR = L"";
	PCWSTR const SPACE = L" ";
	PCWSTR const PATH_SEPARATOR = L"\\"; // Path separator in the file system
	PCWSTR const PATH_SEPARATORS = L"\\/";
	PCWSTR const PATH_PREFIX_NT = L"\\\\?\\"; // Prefix to put ahead of a long path for Windows API
	PCWSTR const REPARSE_PREFIX = L"\\??\\";
	PCWSTR const NETWORK_PATH_PREFIX = L"\\\\";
	PCWSTR const MASK_CHARS = L"*?";

	const wchar_t PATH_SEPARATOR_C = L'\\';
	const wchar_t STR_END_C = L'\0';
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

	typedef ssize_t Timeout_t;

	const Timeout_t WAIT_FOREVER = INFINITE;

	enum class WaitResult_t : size_t {
		SUCCESS   = WAIT_OBJECT_0,
		APC       = WAIT_IO_COMPLETION,
		TIMEOUT   = WAIT_TIMEOUT,
		FAILED    = WAIT_FAILED,
		ABANDONED = WAIT_ABANDONED,
	};

	PCWSTR to_str(WaitResult_t waitResult);

	template<typename Type, size_t N>
	size_t lengthof(Type (&)[N])
	{
		return N;
	}

	inline uint64_t make_uint64(uint32_t high, uint32_t low)
	{
		return uint64_t(high) << 32 | low;
	}

	inline uint32_t high_part_64(uint64_t arg64)
	{
		return uint32_t(arg64 >> 32);
	}

	inline uint32_t low_part_64(uint64_t arg64)
	{
		return uint32_t(arg64 & 0xFFFFFFFFULL);
	}

	inline PCSTR filename_only(PCSTR path, char ch = '\\')
	{
		PCSTR tmp = strrchr((path), ch);
		return (tmp ? tmp : (path) - 1) + 1;
//		return (strrchr((path), ch) ? : (path) - 1) + 1;
	}

	inline PCWSTR filename_only(PCWSTR path, wchar_t ch = PATH_SEPARATOR_C)
	{
		PCWSTR tmp = wcsrchr((path), ch);
		return (tmp ? tmp : (path) - 1) + 1;
//		return (wcsrchr((path), ch) ? : (path) - 1) + 1;
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

	///================================================================================== Uncopyable
	class Uncopyable {
		typedef Uncopyable this_type;

	protected:
		~Uncopyable()
		{
		}

		Uncopyable()
		{
		}

	private:
#ifdef _MSC_VER
		Uncopyable(const this_type &);
#else
		Uncopyable(const this_type &) = delete;
#endif

#ifdef _MSC_VER
		this_type & operator = (const this_type &);
#else
		this_type & operator = (const this_type &) = delete;
#endif
	};

	///=================================================================================== Command_p
	struct Command_p {
		virtual ~Command_p();

		virtual ssize_t execute();
	};

	///================================================================================= Destroyable
	struct Destroyable {
		virtual ~Destroyable();

		virtual void destroy() const = 0;
	};

	///=============================================================================================
	bool safe_vsnprintf(PWSTR buf, size_t len, PCWSTR format, va_list vl);

	bool safe_snprintf(PWSTR buff, size_t len, PCWSTR format, ...);

	bool safe_vsnprintf(PSTR buf, size_t len, PCSTR format, va_list vl);

	bool safe_snprintf(PSTR buff, size_t len, PCSTR format, ...);
}

#endif
