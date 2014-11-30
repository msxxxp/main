#include <basis/sys/cstr.hpp>
#include <basis/sys/sstr.hpp>

#include <basis/simstd/string>

namespace sstr {

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
		return inplace::trim(Result, get_trim_default_uchars());
	}

	ustring CutWordEx(ustring &inout, const ustring &delim, bool delDelim)
	{
		ustring::size_type pos = inout.find(delim);
		ustring Result = inout.substr(0, pos);
		if (delDelim && pos != ustring::npos)
			pos += delim.size();
		inout.erase(0, pos);
		return inplace::trim(Result, get_trim_default_uchars());
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

	ustring copy_after_last(const ustring& in, const ustring& delim)
	{
		ustring::size_type pos = in.find_last_of(delim);
		return (pos != ustring::npos) ? in.substr(pos + 1) : ustring();
	}

	astring to_str_a(int64_t num, int base)
	{
		CHAR buf[64];
		::_i64toa(num, buf, base);
		return astring(buf);
	}

	ustring to_str(int64_t num, int base)
	{
		wchar_t buf[64];
		::_i64tow(num, buf, base);
		return ustring(buf);
	}

	astring oem(const wchar_t* in)
	{
		return sstr::w2cp(in, CP_OEMCP);
	}

	astring oem(const ustring &in)
	{
		return sstr::w2cp(in.c_str(), CP_OEMCP);
	}

	astring ansi(const wchar_t* in)
	{
		return sstr::w2cp(in, CP_ACP);
	}

	astring ansi(const ustring &in)
	{
		return sstr::w2cp(in.c_str(), CP_ACP);
	}

	astring utf8(const wchar_t* in)
	{
		return sstr::w2cp(in, CP_UTF8);
	}

	astring utf8(const ustring &in)
	{
		return sstr::w2cp(in.c_str(), CP_UTF8);
	}

	ustring utf16(const char* in, UINT cp)
	{
		return sstr::cp2w(in, cp);
	}

	ustring utf16(const astring &in, UINT cp)
	{
		return sstr::cp2w(in.c_str(), cp);
	}

	ustring& Add(ustring& str, const wchar_t add)
	{
		auto pos = str.size() - 1;
		if (!(str.empty() || (str.at(pos) == add)))
			str += add;
		return str;
	}

	ustring& Add(ustring& str, const ustring& add)
	{
		auto pos = str.size() - add.size();
		if (!(add.empty() || str.empty() || (str.rfind(add) == pos)))
			str += add;
		return str;
	}

	ustring& Cut(ustring& str, const ustring& sub)
	{
		auto pos = str.find(sub);
		if (pos != ustring::npos) {
			str.erase(pos, sub.size());
		}
		return str;
	}

	bool Cut(ustring& str, intmax_t & num, int base)
	{
		auto pos1 = str.find_first_of(L"0123456789");
		if (pos1 == ustring::npos)
			return false;
		auto pos2 = str.find_first_not_of(L"0123456789", pos1);
		if (pos1 > 0 && str[pos1 - 1] == L'-')
			--pos1;
		ustring tmp(str.substr(pos1, pos2 - pos1));
		num = cstr::to_int64(tmp.c_str(), base);
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
	 wchar_t *				CharFirstOf(const wchar_t* in, const wchar_t* mask) {
	 size_t	lin = Len(in);
	 size_t	len = Len(mask);
	 for (size_t i = 0; i < lin; ++i) {
	 for (size_t j = 0; j < len; ++j) {
	 if (in[i] == mask[j])
	 return (wchar_t *)&in[i];
	 }
	 }
	 return nullptr;
	 }
	 wchar_t *				CharFirstNotOf(const wchar_t* in, const wchar_t* mask) {
	 size_t	lin = Len(in);
	 size_t	len = Len(mask);
	 for (size_t i = 0; i < lin; ++i) {
	 for (size_t j = 0; j < len; ++j) {
	 if (in[i] == mask[j])
	 break;
	 if (j == len - 1)
	 return (wchar_t *)&in[i];
	 }
	 }
	 return nullptr;
	 }
	 */

}
