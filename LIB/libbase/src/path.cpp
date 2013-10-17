#include <libbase/std.hpp>
#include <libbase/path.hpp>
#include <libbase/string.hpp>
#include <libbase/cstr.hpp>

namespace Base {
	namespace Path {
		namespace Inplace {
			ustring & remove_prefix(ustring & path, const ustring & pref)
			{
				if (path.length() >= pref.length() && path.compare(0, pref.length(), pref) == 0)
					path.erase(0, pref.length());
				return path;
			}

			ustring & ensure_prefix(ustring & path, const ustring & pref)
			{
				if (path.compare(0, 2, NETWORK_PATH_PREFIX) != 0 && path.compare(0, pref.length(), pref) != 0)
					path.insert(0, pref);
				return path;
			}

			ustring & ensure_no_end_separator(ustring & path)
			{
				if (!path.empty() && Cstr::find(PATH_SEPARATORS, path[path.size() - 1])) {
					path.erase(path.size() - 1);
				}
				return path;
			}

			ustring & ensure_end_separator(ustring & path, wchar_t sep)
			{
				if (!path.empty() && !Cstr::find(PATH_SEPARATORS, path[path.size() - 1])) {
					path += sep;
				}
				return path;
			}

		}

		ustring get_root(PCWSTR path) {
			wchar_t ret[MAX_PATH];
			if (::GetVolumePathNameW(path, ret, Base::lengthof(ret)))
				return ustring(ret);
			return ustring(path);
		}

		ustring get_work_directory()
		{
			Base::auto_array<wchar_t> Result(::GetCurrentDirectoryW(0, nullptr));
			::GetCurrentDirectoryW(Result.size(), Result.data());
			return ustring(Result.data());
		}

		bool set_work_directory(PCWSTR path)
		{
			return Cstr::is_empty(path) ? false : ::SetCurrentDirectoryW(path);
		}

		extern "C" {
			BOOL WINAPI SHGetSpecialFolderPathW(HWND, LPWSTR, int, BOOL);
		}

		bool is_mask(PCWSTR path)
		{
			ustring tmp(path);
			return tmp.find_first_of(MASK_CHARS) != ustring::npos;
		}

		ustring expand(PCWSTR path)
		{
			wchar_t ret[MAX_PATH_LEN];
			return expand(ret, Base::lengthof(ret), path) ? ustring(ret) : ustring(path);
		}

		ustring extract_from_mask(const ustring & path)
		{
			ustring tmp(path);
			ustring::size_type pos = tmp.find_first_of(MASK_CHARS);
			if (pos != ustring::npos)
				tmp.erase(pos);
			return extract(tmp);
		}

		ustring extract(const ustring & path)
		{
			return path.substr(0, path.find_last_of(PATH_SEPARATORS));
		}

		ustring get_special(int csidl, bool create)
		{
			wchar_t buf[MAX_PATH_LEN];
			return SHGetSpecialFolderPathW(nullptr, buf, csidl, create) ? ustring(buf) : ustring();
		}

		ustring get_fullpath(PCWSTR path)
		{
			wchar_t buf[MAX_PATH_LEN];
			::GetFullPathNameW(path, lengthof(buf), buf, nullptr);
			return ustring(buf);
		}

	}

}
