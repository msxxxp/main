#include <basis/sys/fsys.hpp>
#include <basis/sys/path.hpp>
#include <basis/sys/memory.hpp>
#include <basis/sys/cstr.hpp>
#include <excis/dll.hpp>
#include <excis/filesystem.hpp>
#include <excis/exception.hpp>

extern "C" {
	INT WINAPI SHCreateDirectoryExW(HWND, PCWSTR, PSECURITY_ATTRIBUTES);
}

namespace fsys {

	namespace directory {
		bool is_exist(PCWSTR path) {
			return fsys::is_exist(path) && fsys::is_dir(path);
		}

		void create(PCWSTR path, LPSECURITY_ATTRIBUTES lpsa) {
			CheckApi(create_nt(path, lpsa));
		}

		bool create_full_nt(const ustring& p, LPSECURITY_ATTRIBUTES sa) throw() {
			try {
				ustring path(path::get_fullpath(p));
				path = path::nice(path);
				path = path::ensure_prefix(path::ensure_end_separator(path));

				if (path::get_root(path) == path)
					return false;

				if (create_nt(path.c_str(), sa)) {
					return true;
				}

				size_t pos = path.find(L":");
				if (pos == ustring::npos)
					return false;
				pos = path.find_first_of(PATH_SEPARATORS, pos + 1);
				if (pos == ustring::npos)
					return false;
				do {
					pos = path.find_first_of(PATH_SEPARATORS, pos + 1);
					ustring tmp(path.substr(0, pos));
					if (!create_nt(tmp.c_str(), sa))
						return false;
				} while (pos != ustring::npos);
			} catch (exception::Abstract& e) {
				return false;
			}
			return true;
		}

		void create_full(const ustring& p, LPSECURITY_ATTRIBUTES sa) {
			CheckApi(create_full_nt(p, sa));
		}

		bool create_dir(PCWSTR path, LPSECURITY_ATTRIBUTES lpsa) {
			return ::SHCreateDirectoryExW(nullptr, path, lpsa) == ERROR_SUCCESS;
		}

		void del(PCWSTR path) {
			CheckApi(del_nt(path));
		}
	}

}
