#ifndef LIBBASE_CSTR_TO_NUMBER_HPP_
#define LIBBASE_CSTR_TO_NUMBER_HPP_

#ifndef _MSC_VER
extern "C" {
	long long __MINGW_NOTHROW wcstoll(const wchar_t* __restrict__, wchar_t** __restrict__, int);
	unsigned long long __MINGW_NOTHROW wcstoull(const wchar_t* __restrict__, wchar_t ** __restrict__, int);
}
#endif

namespace cstr {

	inline int64_t to_int64(const char* in)
	{
		return _atoi64(in);
	}

	inline uint32_t to_uint32(const char* in, int base = 10)
	{
		char* end_ptr;
		return ::strtoul(in, &end_ptr, base);
	}

	inline int32_t to_int32(const char* in, int base = 10)
	{
		char* end_ptr;
		return ::strtol(in, &end_ptr, base);
	}

	inline double to_double(const char* in)
	{
		char* end_ptr;
		return ::strtod(in, &end_ptr);
	}

	inline uint64_t to_uint64(const wchar_t* in, int base = 10)
	{
#ifdef _MSC_VER
		UNUSED(base);
		return _wtoi64(in);
#else
		wchar_t* end_ptr;
		return ::wcstoull(in, &end_ptr, base);
#endif
	}

	inline int64_t to_int64(const wchar_t* in, int base = 10)
	{
#ifdef _MSC_VER
		UNUSED(base);
		return _wtoi64(in);
#else
		wchar_t* end_ptr;
		return ::wcstoll(in, &end_ptr, base);
#endif
	}

	inline uint32_t to_uint32(const wchar_t* in, int base = 10)
	{
		wchar_t* end_ptr;
		return ::wcstoul(in, &end_ptr, base);
	}

	inline int32_t to_int32(const wchar_t* in, int base = 10)
	{
		wchar_t* end_ptr;
		return ::wcstol(in, &end_ptr, base);
	}

	inline double to_double(const wchar_t* in)
	{
		wchar_t* end_ptr;
		return ::wcstod(in, &end_ptr);
	}

}

#endif
