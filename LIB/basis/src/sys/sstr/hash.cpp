#include <basis/sys/cstr.hpp>
#include <basis/sys/sstr.hpp>

#include <basis/simstd/string>

namespace sstr {

	astring Hash2Str(const PBYTE hash, size_t size)
	{
		memory::auto_array<char> buf((size + 1) * 2);
		char* tmp = buf.data();
		for (size_t i = 0; i < size; ++i) {
			safe_snprintf(tmp, buf.size() - i * 2, "%02x", hash[i]);
			tmp += 2;
		}
		return astring(buf.data());
	}

	astring Hash2StrNum(const PBYTE hash, size_t size)
	{
		memory::auto_array<char> buf((size + 1) * 4);
		char* tmp = buf.data();
		for (size_t i = 0; i < size; ++i) {
			tmp += safe_snprintf(tmp, buf.size() - i * 2, "%02i ", hash[i]);
		}
		return astring(buf.data());
	}

	bool Str2Hash(const astring &str, PVOID &hash, ULONG &size)
	{
		size_t strsize = str.size();
		if (strsize % 2 == 0) {
			size = strsize / 2;
			hash = memory::malloc<PVOID>(size);
			for (size_t i = 0; i < size; ++i) {
				astring tmp = str.substr(i * 2, 2);
				((PBYTE)hash)[i] = (BYTE)cstr::to_int32(tmp.c_str(), 16);
			}
			return true;
		}
		return false;
	}

}

ustring to_str(const SYSTEMTIME & in, bool tolocal)
{
	SYSTEMTIME stTime;
	if (tolocal) {
		::SystemTimeToTzSpecificLocalTime(nullptr, (SYSTEMTIME*)&in, &stTime);
	} else {
		stTime = in;
	}
	wchar_t buf[MAX_PATH];
	_snwprintf(buf, lengthof(buf), L"%04d-%02d-%02d %02d:%02d:%02d", stTime.wYear, stTime.wMonth, stTime.wDay, stTime.wHour, stTime.wMinute, stTime.wSecond);
	return buf;
}

ustring to_str(const FILETIME & in)
{
	SYSTEMTIME stUTC;
	::FileTimeToSystemTime(&in, &stUTC);
	return to_str(stUTC);
}

ustring to_str(const PBYTE hash, size_t size)
{
	memory::auto_array<wchar_t> buf((size + 1) * 2);
	wchar_t* tmp = buf.data();
	for (size_t i = 0; i < size; ++i) {
		safe_snprintf(tmp, buf.size() - i * 2, L"%02x", hash[i]);
		tmp += 2;
	}
	return ustring(buf.data());
}

memory::auto_array<BYTE> to_hash(const ustring& str)
{
	size_t size = str.size() / 2;
	memory::auto_array<BYTE> ret(size);
	for (size_t i = 0; i < size; ++i) {
		ustring tmp = str.substr(i * 2, 2);
		ret[i] = (BYTE)cstr::to_int32(tmp.c_str(), 16);
	}
	return ret;
}

void to_hash(const ustring& str, PBYTE & buf, size_t & size)
{
	memory::auto_array < BYTE > (to_hash(str)).detach(buf, size);
}
