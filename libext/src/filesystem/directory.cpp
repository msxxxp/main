#include <libbase/std.hpp>
#include <libbase/memory.hpp>
#include <libbase/path.hpp>
#include <libbase/pcstr.hpp>
#include <libext/dll.hpp>
#include <libext/filesystem.hpp>
#include <libext/exception.hpp>

extern "C" {
	INT WINAPI SHCreateDirectoryExW(HWND, PCWSTR, PSECURITY_ATTRIBUTES);
}

namespace Ext {
	namespace Directory {
		bool is_exist(PCWSTR path) {
			return FS::is_exist(path) && FS::is_dir(path);
		}

		bool create_nt(PCWSTR path, LPSECURITY_ATTRIBUTES lpsa) {
			return ::CreateDirectoryW(path, lpsa) || (::GetLastError() == ERROR_ALREADY_EXISTS && FS::is_dir(path));
		}

		void create(PCWSTR path, LPSECURITY_ATTRIBUTES lpsa) {
			CheckApi(create_nt(path, lpsa));
		}

		bool create_full_nt(const ustring & p, LPSECURITY_ATTRIBUTES sa) throw() {
			try {
				ustring path(Base::Path::get_fullpath(p));
				path = Base::PathNice(path);
				path = Base::Path::ensure_prefix(Base::Path::ensure_end_separator(path));

				if (Base::Path::get_root(path) == path)
					return false;

				if (create_nt(path.c_str(), sa)) {
					return true;
				}

				size_t pos = path.find(L":");
				if (pos == ustring::npos)
					return false;
				pos = path.find_first_of(Base::PATH_SEPARATORS, pos + 1);
				if (pos == ustring::npos)
					return false;
				do {
					pos = path.find_first_of(Base::PATH_SEPARATORS, pos + 1);
					ustring tmp(path.substr(0, pos));
					if (!create_nt(tmp.c_str(), sa))
						return false;
				} while (pos != ustring::npos);
			} catch (AbstractError & e) {
				return false;
			}
			return true;
		}

		void create_full(const ustring & p, LPSECURITY_ATTRIBUTES sa) {
			CheckApi(create_full_nt(p, sa));
		}

		bool create_dir(PCWSTR path, LPSECURITY_ATTRIBUTES lpsa) {
			return ::SHCreateDirectoryExW(nullptr, path, lpsa) == ERROR_SUCCESS;
		}

		bool del_simple_nt(PCWSTR path)
		{
			return ::RemoveDirectoryW(path);
		}

		bool del_attrcheck_nt(PCWSTR path)
		{
			DWORD attr = ::GetFileAttributesW(path);
			if (attr != INVALID_FILE_ATTRIBUTES && ::SetFileAttributesW(path, FILE_ATTRIBUTE_NORMAL)) {
				if (del_simple_nt(path))
					return true;
				else
					::SetFileAttributesW(path, attr);
			}
			return false;
		}

		bool del_nt(PCWSTR path)
		{
			return del_simple_nt(path) || del_attrcheck_nt(path);
		}

		void del(PCWSTR path) {
			CheckApi(del_nt(path));
		}
	}

}
