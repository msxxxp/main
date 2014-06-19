#include <basis/std/string>

#include <basis/sys/cstr.hpp>

namespace simstd {

	string to_string(int value)
	{
		return to_string(static_cast<long long>(value));
	}

	string to_string(long value)
	{
		return to_string(static_cast<long long>(value));
	}

	string to_string(long long value)
	{
		char buffer[64];
		cstr::to_string(buffer, lengthof(buffer), value);
		return string(buffer);
	}

	string to_string(unsigned value)
	{
		return to_string(static_cast<unsigned long long>(value));
	}

	string to_string(unsigned long value)
	{
		return to_string(static_cast<unsigned long long>(value));
	}

	string to_string(unsigned long long value)
	{
		char buffer[64];
		cstr::to_string(buffer, lengthof(buffer), value);
		return string(buffer);
	}

	string to_string(float value)
	{
		char buffer[128];
		cstr::to_string(buffer, lengthof(buffer), value);
		return string(buffer);
	}

	string to_string(double value)
	{
		char buffer[128];
		cstr::to_string(buffer, lengthof(buffer), value);
		return string(buffer);
	}

	string to_string(long double value)
	{
		char buffer[128];
		cstr::to_string(buffer, lengthof(buffer), value);
		return string(buffer);
	}

	int stoi(const string& str, size_t* pos, int base)
	{
		return cstr::stoi(str.c_str(), pos, base);
	}

	long stol(const string& str, size_t* pos, int base)
	{
		return cstr::stol(str.c_str(), pos, base);
	}

	long long stoll(const string& str, size_t* pos, int base)
	{
		return cstr::stoll(str.c_str(), pos, base);
	}

	unsigned long stoul(const string& str, size_t* pos, int base)
	{
		return cstr::stoul(str.c_str(), pos, base);
	}

	unsigned long long stoull(const string& str, size_t* pos, int base)
	{
		return cstr::stoull(str.c_str(), pos, base);
	}

	float stof(const string& str, size_t* pos)
	{
		return static_cast<float>(stod(str, pos));
	}

	double stod(const string& str, size_t* pos)
	{
		return cstr::stod(str.c_str(), pos);
	}

	long double stold(const string& str, size_t* pos)
	{
		return cstr::stold(str.c_str(), pos);
	}

}
