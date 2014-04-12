#ifndef LIBSTL_CSTR_CONVERT_A_HPP_
#define LIBSTL_CSTR_CONVERT_A_HPP_

#include <system/base_types.hpp>

namespace cstr {

	void to_string(char * str, size_t len, int value);

	void to_string(char * str, size_t len, long value);

	void to_string(char * str, size_t len, long long value);

	void to_string(char * str, size_t len, unsigned value);

	void to_string(char * str, size_t len, unsigned long value);

	void to_string(char * str, size_t len, unsigned long long value);

	void to_string(char * str, size_t len, float value);

	void to_string(char * str, size_t len, double value);

	void to_string(char * str, size_t len, long double value);

	int stoi(const char * str, size_t * pos = nullptr, int base = 10);

	long stol(const char * str, size_t * pos = nullptr, int base = 10);

	long long stoll(const char * str, size_t * pos = nullptr, int base = 10);

	unsigned long stoul(const char * str, size_t * pos = nullptr, int base = 10);

	unsigned long long stoull(const char * str, size_t * pos = nullptr, int base = 10);

	float stof(const char * str, size_t * pos = nullptr);

	double stod(const char * str, size_t * pos = nullptr);

	long double stold(const char * str, size_t * pos = nullptr);
}

#endif
