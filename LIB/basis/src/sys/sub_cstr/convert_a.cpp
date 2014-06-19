#include <basis/sys/cstr.hpp>
#include <basis/sys/memory.hpp>

namespace cstr {

	void to_string(char * str, size_t len, int value)
	{
		to_string(str, len, static_cast<long long>(value));
	}

	void to_string(char * str, size_t len, long value)
	{
		to_string(str, len, static_cast<long long>(value));
	}

	void to_string(char * str, size_t len, long long value)
	{
		memory::zero(str, len);
		::_i64toa(value, str, 10);
	}

	void to_string(char * str, size_t len, unsigned value)
	{
		to_string(str, len, static_cast<unsigned long long>(value));
	}

	void to_string(char * str, size_t len, unsigned long value)
	{
		to_string(str, len, static_cast<unsigned long long>(value));
	}

	void to_string(char * str, size_t len, unsigned long long value)
	{
		memory::zero(str, len);
		::_ui64toa(value, str, 10);
	}

	void to_string(char * str, size_t len, float value)
	{
		memory::zero(str, len);
		safe_snprintf(str, len, "%f", value);
	}

	void to_string(char * str, size_t len, double value)
	{
		memory::zero(str, len);
		safe_snprintf(str, len, "%lf", value);
	}

	void to_string(char * str, size_t len, long double value)
	{
		memory::zero(str, len);
		safe_snprintf(str, len, "%llf", value); // not work
	}

	int stoi(const char * str, size_t * pos, int base)
	{
		char * end_ptr;
		int ret = ::strtol(str, &end_ptr, base);
		if (pos)
			*pos = end_ptr - str;
		return ret;
	}

	long stol(const char * str, size_t * pos, int base)
	{
		char * end_ptr;
		long ret = ::strtol(str, &end_ptr, base);
		if (pos)
			*pos = end_ptr - str;
		return ret;
	}

	long long stoll(const char * str, size_t * pos, int base)
	{
		char * end_ptr;
		long long ret = ::strtoll(str, &end_ptr, base);
		if (pos)
			*pos = end_ptr - str;
		return ret;
	}

	unsigned long stoul(const char * str, size_t * pos, int base)
	{
		char * end_ptr;
		unsigned long ret = ::strtoul(str, &end_ptr, base);
		if (pos)
			*pos = end_ptr - str;
		return ret;
	}

	unsigned long long stoull(const char * str, size_t * pos, int base)
	{
		char * end_ptr;
		unsigned long long ret = ::strtoull(str, &end_ptr, base);
		if (pos)
			*pos = end_ptr - str;
		return ret;
	}

	float stof(const char * str, size_t * pos)
	{
		return static_cast<float>(stod(str, pos));
	}

	double stod(const char * str, size_t * pos)
	{
		char * end_ptr;
		double ret = ::strtod(str, &end_ptr);
		if (pos)
			*pos = end_ptr - str;
		return ret;
	}

	long double stold(const char * str, size_t * pos)
	{
		char * end_ptr;
		long double ret = ::strtold(str, &end_ptr);
		if (pos)
			*pos = end_ptr - str;
		return ret;
	}

}
