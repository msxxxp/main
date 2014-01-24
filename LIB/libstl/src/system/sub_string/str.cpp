#include <system/string.hpp>
#include <system/cstr.hpp>

#include <simstd/string>

namespace String {

	namespace Inplace {
		astring & trim_left(astring & str, const astring & chrs)
		{
			astring::size_type pos = str.find_first_not_of(chrs);
			if (pos && pos != astring::npos) {
				str.erase(0, pos);
			}
			return str;
		}

		ustring & trim_left(ustring & str, const ustring & chrs)
		{
			ustring::size_type pos = str.find_first_not_of(chrs);
			if (pos && pos != ustring::npos) {
				str.erase(0, pos);
			}
			return str;
		}

		astring & trim_right(astring & str, const astring & chrs)
		{
			astring::size_type pos = str.find_last_not_of(chrs);
			if (pos != astring::npos && (++pos < str.size())) {
				str.erase(pos);
			}
			return str;
		}

		ustring & trim_right(ustring & str, const ustring & chrs)
		{
			ustring::size_type pos = str.find_last_not_of(chrs);
			if (pos != ustring::npos && (++pos < str.size())) {
				str.erase(pos);
			}
			return str;
		}

		astring & trim(astring & str, const astring & chrs)
		{
			trim_right(str, chrs);
			trim_left(str, chrs);
			return str;
		}

		ustring & trim(ustring & str, const ustring & chrs)
		{
			trim_right(str, chrs);
			trim_left(str, chrs);
			return str;
		}

		ustring & replace_all(ustring & str, const ustring & from, const ustring & to)
		{
			ustring::size_type pos;
			while ((pos = str.find(from)) != ustring::npos) {
				str.replace(pos, from.size(), to);
			}
			return str;
		}

		ustring & to_lower(ustring & inout)
		{
			if (!inout.empty()) {
				::CharLowerW((wchar_t*)inout.c_str());
			}
			return inout;
		}

		ustring & to_upper(ustring & inout)
		{
			if (!inout.empty()) {
				::CharUpperW((wchar_t*)inout.c_str());
			}
			return inout;
		}

		ustring & add_word(ustring & inout, const ustring & add, const ustring & delim)
		{
			// добаваляет строку через разделитель кроме случаев
			// 1) исходная строка пуста
			// 2) если добавляемая строка пуста
			// 3) если разделитель есть в конце исходной строки
			// 4) если разделитель есть в начале добавляемой
			ustring::size_type pos = inout.size() - delim.size();
			if (!(add.empty() || delim.empty() || inout.empty() || (inout.rfind(delim) == pos) || (add.find(delim) == 0)))
				inout += delim;
			if (!add.empty())
				inout += add;
			return inout;
		}

	}

	ustring GetWord(const ustring &str, wchar_t d)
	{
		ustring::size_type pos = str.find(d);
		if (pos != ustring::npos)
			return str.substr(0, pos);
		return str;
	}

	ustring CutWord(ustring &inout, const ustring &delim, bool delDelim)
	{
		ustring::size_type pos = inout.find_first_of(delim);
		ustring Result(inout.substr(0, pos));
		if (delDelim && pos != ustring::npos) {
			//		pos = inout.find_first_not_of(delim, pos);
			++pos;
		}
		inout.erase(0, pos);
		return Inplace::trim(Result);
	}

	ustring CutWordEx(ustring &inout, const ustring &delim, bool delDelim)
	{
		ustring::size_type pos = inout.find(delim);
		ustring Result = inout.substr(0, pos);
		if (delDelim && pos != ustring::npos)
			pos += delim.size();
		inout.erase(0, pos);
		return Inplace::trim(Result);
	}

//	UINT CheckUnicode(const PVOID buf, size_t size)
//	{
//		int test = 0xFFFF;
//		if (::IsTextUnicode(buf, (int)size, &test)) {
//			if (test & IS_TEXT_UNICODE_UNICODE_MASK) {
//				return CP_UTF16le;
//			} else if (test & IS_TEXT_UNICODE_REVERSE_MASK) {
//				return CP_UTF16be;
//			}
//			/*
//			 if (!(test&IS_TEXT_UNICODE_ODD_LENGTH) && !(test&IS_TEXT_UNICODE_ILLEGAL_CHARS)) {
//			 if ((test&IS_TEXT_UNICODE_NULL_BYTES) ||
//			 (test&IS_TEXT_UNICODE_CONTROLS) ||
//			 (test&IS_TEXT_UNICODE_REVERSE_CONTROLS)) {
//			 if ((test&IS_TEXT_UNICODE_CONTROLS) || (test&IS_TEXT_UNICODE_STATISTICS)) {
//			 nCodePage = CP_UTF16le;
//			 bDetect = true;
//			 } else if ((test&IS_TEXT_UNICODE_REVERSE_CONTROLS) || (test&IS_TEXT_UNICODE_REVERSE_STATISTICS)) {
//			 nCodePage = CP_UTF16be;
//			 bDetect = true;
//			 }
//			 }
//			 }
//			 */
//		}
//		return 0;
//	}

	UINT IsUTF8(const PVOID buf, size_t size)
	{
		bool Ascii = true;
		UINT Octets = 0;
		for (size_t i = 0; i < size; ++i) {
			BYTE c = ((PBYTE)buf)[i];
			if (c & 0x80)
				Ascii = 0;
			if (Octets) {
				if ((c & 0xC0) != 0x80)
					return 0;
				Octets--;
			} else {
				if (c & 0x80) {
					while (c & 0x80) {
						c <<= 1;
						Octets++;
					}
					Octets--;
					if (!Octets)
						return 0;
				}
			}
		}
		return (Octets > 0 || Ascii) ? 0 : CP_UTF8;
	}

//	bool GetCP(HANDLE hFile, UINT &cp, bool bUseHeuristics)
//	{
//		DWORD dwTemp = 0;
//		DWORD size = 0;
//
//		::SetFilePointer(hFile, 0, nullptr, FILE_BEGIN);
//		if (::ReadFile(hFile, &dwTemp, sizeof(dwTemp), &size, nullptr)) {
//			if (LOWORD(dwTemp) == BOM_UTF16le) {
//				::SetFilePointer(hFile, 2, nullptr, FILE_BEGIN);
//				cp = CP_UTF16le;
//				return true;
//			} else if (LOWORD(dwTemp) == BOM_UTF16be) {
//				::SetFilePointer(hFile, 2, nullptr, FILE_BEGIN);
//				cp = CP_UTF16be;
//				return true;
//			} else if ((dwTemp & 0x00FFFFFF) == BOM_UTF8) {
//				::SetFilePointer(hFile, 3, nullptr, FILE_BEGIN);
//				cp = CP_UTF8;
//				return true;
//			} else if (dwTemp == BOM_UTF32le) {
//				cp = CP_UTF32le;
//				return true;
//			} else if (dwTemp == BOM_UTF32be) {
//				cp = CP_UTF32be;
//				return true;
//			} else {
//				::SetFilePointer(hFile, 0, nullptr, FILE_BEGIN);
//			}
//		}
//
//		UINT nCodePage = 0;
//		if (bUseHeuristics) {
//			//		size = 0x8000; // BUGBUG.
//			//		PBYTE 	buf;
//			//		if (Memory::Alloc(buf, size) && read_file(hFile, buf, size)) {
//			//			UINT	cp = CheckUnicode(buf, size);
//			//			if (cp) {
//			//				nCodePage = cp;
//			//			} else if (IsUTF8(buf, size)) {
//			//				nCodePage = CP_UTF8;
//			//			} else {
//			//				/*
//			//				nsUniversalDetectorEx *ns = new nsUniversalDetectorEx();
//			//
//			//				ns->HandleData((const char*)Buffer, (PRUint32)sz);
//			//				ns->DataEnd();
//			//
//			//				int cp = ns->getCodePage();
//			//
//			//				if (cp != -1) {
//			//					nCodePage = cp;
//			//					bDetect = true;
//			//				}
//			//
//			//				delete ns;
//			//				*/
//			//			}
//			//			Memory::Free(buf);
//			//		}
//		}
//		return nCodePage;
//	}

	ustring copy_after_last(const ustring & in, const ustring & delim)
	{
		ustring::size_type pos = in.find_last_of(delim);
		return (pos != ustring::npos) ? in.substr(pos + 1) : ustring();
	}

	ustring & Add(ustring & str, const wchar_t add)
	{
		auto pos = str.size() - 1;
		if (!(str.empty() || (str.at(pos) == add)))
			str += add;
		return str;
	}

	ustring & Add(ustring & str, const ustring & add)
	{
		auto pos = str.size() - add.size();
		if (!(add.empty() || str.empty() || (str.rfind(add) == pos)))
			str += add;
		return str;
	}

	ustring & Cut(ustring & str, const ustring & sub)
	{
		auto pos = str.find(sub);
		if (pos != ustring::npos) {
			str.erase(pos, sub.size());
		}
		return str;
	}

	bool Cut(ustring & str, intmax_t & num, int base)
	{
		auto pos1 = str.find_first_of(L"0123456789");
		if (pos1 == ustring::npos)
			return false;
		auto pos2 = str.find_first_not_of(L"0123456789", pos1);
		if (pos1 > 0 && str[pos1 - 1] == L'-')
			--pos1;
		ustring tmp(str.substr(pos1, pos2 - pos1));
		num = Cstr::to_int64(tmp.c_str(), base);
		str.erase(0, pos2);
		return true;
	}

	//bool		Cut(ustring &inout, intmax_t &num, int base) {
	//	return inout.Cut(num, base);
	//}
	//
	//ustring&	CutAfter(ustring &inout, const ustring &delim) {
	//	ustring::size_type pos = inout.find_first_of(delim);
	//	if (pos != ustring::npos) {
	//		inout.erase(pos);
	//	}
	//	return inout;
	//}
	//
	//ustring&	CutBefore(ustring &inout, const ustring &delim) {
	//	size_t	pos = inout.find_first_of(delim);
	//	if (pos != 0) {
	//		inout.erase(0, pos);
	//	}
	//	return inout;
	//}

	/*
	 PWSTR				CharFirstOf(PCWSTR in, PCWSTR mask) {
	 size_t	lin = Len(in);
	 size_t	len = Len(mask);
	 for (size_t i = 0; i < lin; ++i) {
	 for (size_t j = 0; j < len; ++j) {
	 if (in[i] == mask[j])
	 return (PWSTR)&in[i];
	 }
	 }
	 return nullptr;
	 }
	 PWSTR				CharFirstNotOf(PCWSTR in, PCWSTR mask) {
	 size_t	lin = Len(in);
	 size_t	len = Len(mask);
	 for (size_t i = 0; i < lin; ++i) {
	 for (size_t j = 0; j < len; ++j) {
	 if (in[i] == mask[j])
	 break;
	 if (j == len - 1)
	 return (PWSTR)&in[i];
	 }
	 }
	 return nullptr;
	 }
	 */

	astring Hash2Str(const PBYTE hash, size_t size)
	{
		memory::auto_array<char> buf((size + 1) * 2);
		PSTR tmp = buf.data();
		for (size_t i = 0; i < size; ++i) {
			safe_snprintf(tmp, buf.size() - i * 2, "%02x", hash[i]);
			tmp += 2;
		}
		return astring(buf.data());
	}

	astring Hash2StrNum(const PBYTE hash, size_t size)
	{
		memory::auto_array<char> buf((size + 1) * 4);
		PSTR tmp = buf.data();
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
			hash = memory::malloc < PVOID > (size);
			for (size_t i = 0; i < size; ++i) {
				astring tmp = str.substr(i * 2, 2);
				((PBYTE)hash)[i] = (BYTE)Cstr::to_int32(tmp.c_str(), 16);
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
	PWSTR tmp = buf.data();
	for (size_t i = 0; i < size; ++i) {
		safe_snprintf(tmp, buf.size() - i * 2, L"%02x", hash[i]);
		tmp += 2;
	}
	return ustring(buf.data());
}

memory::auto_array<BYTE> to_hash(const ustring & str)
{
	size_t size = str.size() / 2;
	memory::auto_array<BYTE> ret(size);
	for (size_t i = 0; i < size; ++i) {
		ustring tmp = str.substr(i * 2, 2);
		ret[i] = (BYTE)Cstr::to_int32(tmp.c_str(), 16);
	}
	return ret;
}

void to_hash(const ustring & str, PBYTE & buf, size_t & size)
{
	memory::auto_array < BYTE > (to_hash(str)).detach(buf, size);
}
