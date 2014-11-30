#include <basis/sys/cstr.hpp>
#include <basis/sys/memory.hpp>

//#ifndef _MSC_VER
//extern "C" {
//	long long __MINGW_NOTHROW wcstoll(const wchar_t* __restrict__, wchar_t** __restrict__, int);
//	unsigned long long __MINGW_NOTHROW wcstoull(const wchar_t* __restrict__, wchar_t ** __restrict__, int);
//}
//#endif

namespace cstr {

	void to_wstring(wchar_t* str, size_t len, int value)
	{
		to_wstring(str, len, static_cast<long long>(value));
	}

	void to_wstring(wchar_t* str, size_t len, long value)
	{
		to_wstring(str, len, static_cast<long long>(value));
	}

	void to_wstring(wchar_t* str, size_t len, long long value)
	{
		memory::zero(str, sizeof(wchar_t) * len);
		::_i64tow(value, str, 10);
	}

	void to_wstring(wchar_t* str, size_t len, unsigned value)
	{
		to_wstring(str, len, static_cast<unsigned long long>(value));
	}

	void to_wstring(wchar_t* str, size_t len, unsigned long value)
	{
		to_wstring(str, len, static_cast<unsigned long long>(value));
	}

	void to_wstring(wchar_t* str, size_t len, unsigned long long value)
	{
		memory::zero(str, sizeof(wchar_t) * len);
		::_ui64tow(value, str, 10);
	}

	void to_wstring(wchar_t* str, size_t len, float value)
	{
		memory::zero(str, sizeof(wchar_t) * len);
		safe_snprintf(str, len, L"%f", value);
	}

	void to_wstring(wchar_t* str, size_t len, double value)
	{
		memory::zero(str, sizeof(wchar_t) * len);
		safe_snprintf(str, len, L"%lf", value);
	}

	void to_wstring(wchar_t* str, size_t len, long double value)
	{
		memory::zero(str, sizeof(wchar_t) * len);
		safe_snprintf(str, len, L"%llf", value); // not work
	}

	int stoi(const wchar_t* str, size_t * pos, int base)
	{
		wchar_t* end_ptr;
		int ret = ::wcstol(str, &end_ptr, base);
		if (pos)
			*pos = end_ptr - str;
		return ret;
	}

	long stol(const wchar_t* str, size_t * pos, int base)
	{
		wchar_t* end_ptr;
		long ret = ::wcstol(str, &end_ptr, base);
		if (pos)
			*pos = end_ptr - str;
		return ret;
	}

	long long stoll(const wchar_t* str, size_t * pos, int base)
	{
		wchar_t* end_ptr;
		long long ret = ::_wcstoi64(str, &end_ptr, base); // wcstoll
		if (pos)
			*pos = end_ptr - str;
		return ret;
	}

	unsigned long stoul(const wchar_t* str, size_t * pos, int base)
	{
		wchar_t* end_ptr;
		unsigned long ret = ::wcstoul(str, &end_ptr, base);
		if (pos)
			*pos = end_ptr - str;
		return ret;
	}

	unsigned long long stoull(const wchar_t* str, size_t * pos, int base)
	{
		wchar_t* end_ptr;
		unsigned long long ret = ::_wcstoui64(str, &end_ptr, base); //wcstoull
		if (pos)
			*pos = end_ptr - str;
		return ret;
	}

	float stof(const wchar_t* str, size_t * pos)
	{
		return static_cast<float>(stod(str, pos));
	}

	double stod(const wchar_t* str, size_t * pos)
	{
		wchar_t* end_ptr;
		double ret = ::wcstod(str, &end_ptr);
		if (pos)
			*pos = end_ptr - str;
		return ret;
	}

	long double stold(const wchar_t* str, size_t * pos)
	{
		wchar_t* end_ptr;
		long double ret = ::wcstold(str, &end_ptr);
		if (pos)
			*pos = end_ptr - str;
		return ret;
	}

}
