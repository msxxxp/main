#include <basis/std/string>

#include <basis/sys/cstr.hpp>

namespace simstd {

	wstring to_wstring(int value)
	{
		return to_wstring(static_cast<long long>(value));
	}

	wstring to_wstring(long value)
	{
		return to_wstring(static_cast<long long>(value));
	}

	wstring to_wstring(long long value)
	{
		wchar_t buffer[64];
		cstr::to_wstring(buffer, lengthof(buffer), value);
		return wstring(buffer);
	}

	wstring to_wstring(unsigned value)
	{
		return to_wstring(static_cast<unsigned long long>(value));
	}

	wstring to_wstring(unsigned long value)
	{
		return to_wstring(static_cast<unsigned long long>(value));
	}

	wstring to_wstring(unsigned long long value)
	{
		wchar_t buffer[64];
		cstr::to_wstring(buffer, lengthof(buffer), value);
		return wstring(buffer);
	}

	wstring to_wstring(float value)
	{
		wchar_t buffer[128];
		cstr::to_wstring(buffer, lengthof(buffer), value);
		return wstring(buffer);
	}

	wstring to_wstring(double value)
	{
		wchar_t buffer[128];
		cstr::to_wstring(buffer, lengthof(buffer), value);
		return wstring(buffer);
	}

	wstring to_wstring(long double value)
	{
		wchar_t buffer[128];
		cstr::to_wstring(buffer, lengthof(buffer), value);
		return wstring(buffer);
	}

	int stoi(const wstring& str, size_t* pos, int base)
	{
		return cstr::stoi(str.c_str(), pos, base);
	}

	long stol(const wstring& str, size_t* pos, int base)
	{
		return cstr::stol(str.c_str(), pos, base);
	}

	long long stoll(const wstring& str, size_t* pos, int base)
	{
		return cstr::stoll(str.c_str(), pos, base);
	}

	unsigned long stoul(const wstring& str, size_t* pos, int base)
	{
		return cstr::stoul(str.c_str(), pos, base);
	}

	unsigned long long stoull(const wstring& str, size_t* pos, int base)
	{
		return cstr::stoull(str.c_str(), pos, base);
	}

	float stof(const wstring& str, size_t* pos)
	{
		return static_cast<float>(stod(str, pos));
	}

	double stod(const wstring& str, size_t* pos)
	{
		return cstr::stod(str.c_str(), pos);
	}

	long double stold(const wstring& str, size_t* pos)
	{
		return cstr::stold(str.c_str(), pos);
	}

}
