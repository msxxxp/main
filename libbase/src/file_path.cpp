#include <libbase/std.hpp>
#include <libbase/path.hpp>
#include <libbase/string.hpp>
#include <libbase/win64.hpp>

namespace Base {

	ustring MakePath(PCWSTR path, PCWSTR name) {
		ustring Result(PathNice(Path::ensure_end_separator(path)));
		return String::Inplace::add_word(Result, name, PATH_SEPARATOR);
	}

	ustring PathNice(PCWSTR path) {
		return Path::canonicalize(Path::expand(path));
	}

	ustring Secure(PCWSTR path) {
		ustring Result(path);
		String::replace_all(Result, L"..", L"");
		String::replace_all(Result, L"%", L"");
		return Validate(Result);
	}

	ustring Validate(PCWSTR path) {
		ustring Result(Path::canonicalize(Path::expand(path)));
		String::replace_all(Result, L"...", L"");
		String::replace_all(Result, L"\\\\", L"\\");
		if (Result == L"\\")
			Result.clear();
		return Result;
	}

	ustring TempDir() {
		wchar_t buf[MAX_PATH];
		buf[0] = 0;
		::GetTempPathW(sizeofa(buf), buf);
		return ustring(buf);
	}

	ustring TempFile(PCWSTR path) {
		wchar_t buf[MAX_PATH];
		wchar_t pid[32];
		buf[0] = 0;
		Str::convert_num(pid, ::GetCurrentProcessId());
		::GetTempFileNameW(path, pid, 0, buf);
		return buf;
	}

	///========================================================================================= SysPath
	namespace SysPath {
		ustring Winnt() {
			const int CSIDL_WINDOWS = 0x0024;
			ustring ret(Path::get_special(CSIDL_WINDOWS));
			return Path::ensure_end_separator(ret);
		}
		ustring Sys32() {
			const int CSIDL_SYSTEM = 0x0025;
			ustring ret(Path::get_special(CSIDL_SYSTEM));
			return Path::ensure_end_separator(ret);
		}

		ustring SysNative() {
			return PathNice(is_WOW64() ? L"%SystemRoot%\\sysnative\\" : L"%SystemRoot%\\system32\\");
		}

		ustring InetSrv() {
			return MakePath(Sys32().c_str(), L"inetsrv\\");
		}

		ustring Dns() {
			return MakePath(SysNative().c_str(), L"dns\\");
		}

		ustring Temp() {
			return PathNice(L"%TEMP%\\");
		}

		ustring Users() {
			ustring ret = PathNice(L"%PUBLIC%\\..\\");
			return (ret.empty() || (ret == L"\\")) ? PathNice(L"%ALLUSERSPROFILE%\\..\\") : ret;
		}
	}

	///========================================================================================== SysApp
	namespace SysApp {
		ustring appcmd() {
			return SysPath::InetSrv() + L"appcmd.exe ";
		}
	}

	///=================================================================================================
	bool substr_match(const ustring& str, size_t pos, PCWSTR mstr) {
		size_t mstr_len = Str::length(mstr);
		if ((pos > str.size()) || (pos + mstr_len > str.size())) {
			return false;
		}
		return wmemcmp(str.c_str() + pos, mstr, mstr_len) == 0;
	}

	void locate_path_root(const ustring& path, size_t& path_root_len, bool& is_unc_path) {
		unsigned prefix_len = 0;
		is_unc_path = false;
		if (substr_match(path, 0, L"\\\\")) {
			if (substr_match(path, 2, L"?\\UNC\\")) {
				prefix_len = 8;
				is_unc_path = true;
			} else if (substr_match(path, 2, L"?\\") || substr_match(path, 2, L".\\")) {
				prefix_len = 4;
			} else {
				prefix_len = 2;
				is_unc_path = true;
			}
		}
		if ((prefix_len == 0) && !substr_match(path, 1, L":\\")) {
			path_root_len = 0;
		} else {
			ustring::size_type p = path.find(L'\\', prefix_len);
			if (p == ustring::npos) {
				p = path.size();
			}
			if (is_unc_path) {
				p = path.find(L'\\', p + 1);
				if (p == ustring::npos) {
					p = path.size();
				}
			}
			path_root_len = p;
		}
	}

	ustring extract_file_name(const ustring& path) {
		size_t pos = path.rfind(L"\\");
		if (pos == ustring::npos) {
			pos = 0;
		} else {
			pos++;
		}
		size_t path_root_len;
		bool is_unc_path;
		locate_path_root(path, path_root_len, is_unc_path);
		if ((pos <= path_root_len) && (path_root_len != 0))
			return ustring();
		else
			return path.substr(pos);
	}

}
