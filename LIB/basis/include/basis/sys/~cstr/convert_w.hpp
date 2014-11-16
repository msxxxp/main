#ifndef BASIS_CSTR_CONVERT_W_HPP_
#define BASIS_CSTR_CONVERT_W_HPP_

namespace cstr {

	void to_wstring(wchar_t * str, size_t len, int value);

	void to_wstring(wchar_t * str, size_t len, long value);

	void to_wstring(wchar_t * str, size_t len, long long value);

	void to_wstring(wchar_t * str, size_t len, unsigned value);

	void to_wstring(wchar_t * str, size_t len, unsigned long value);

	void to_wstring(wchar_t * str, size_t len, unsigned long long value);

	void to_wstring(wchar_t * str, size_t len, float value);

	void to_wstring(wchar_t * str, size_t len, double value);

	void to_wstring(wchar_t * str, size_t len, long double value);

	int stoi(const wchar_t * str, size_t * pos = nullptr, int base = 10);

	long stol(const wchar_t * str, size_t * pos = nullptr, int base = 10);

	long long stoll(const wchar_t * str, size_t * pos = nullptr, int base = 10);

	unsigned long stoul(const wchar_t * str, size_t * pos = nullptr, int base = 10);

	unsigned long long stoull(const wchar_t * str, size_t * pos = nullptr, int base = 10);

	float stof(const wchar_t * str, size_t * pos = nullptr);

	double stod(const wchar_t * str, size_t * pos = nullptr);

	long double stold(const wchar_t * str, size_t * pos = nullptr);
}

#endif
