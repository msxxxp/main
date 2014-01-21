#ifndef LIBSTL_STRING_CONVERT_A_HPP_
#define LIBSTL_STRING_CONVERT_A_HPP_

#include <simstl/impl/string/base.hpp>
#include <simstl/types.hpp>

namespace simstd {

	string to_string(int value);

	string to_string(long value);

	string to_string(long long value);

	string to_string(unsigned value);

	string to_string(unsigned long value);

	string to_string(unsigned long long value);

	string to_string(float value);

	string to_string(double value);

	string to_string(long double value);

	int stoi(const string& str, size_t *pos = 0, int base = 10);

	long stol(const string& str, size_t *pos = 0, int base = 10);

	long long stoll(const string& str, size_t *pos = 0, int base = 10);

	unsigned long stoul(const string& str, size_t *pos = 0, int base = 10);

	unsigned long long stoull(const string& str, size_t *pos = 0, int base = 10);

	float stof(const string& str, size_t *pos = 0);

	double stod(const string& str, size_t *pos = 0);

	long double stold(const string& str, size_t *pos = 0);
}

#endif
