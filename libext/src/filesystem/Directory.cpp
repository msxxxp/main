#include <libbase/std.hpp>
#include <libbase/filesystem.hpp>
#include <libbase/memory.hpp>
#include <libbase/path.hpp>
#include <libbase/pcstr.hpp>
#include <libext/dll.hpp>
#include <libext/filesystem.hpp>
#include <libext/exception.hpp>

extern "C" {
	INT WINAPI SHCreateDirectoryExW(HWND, PCWSTR, PSECURITY_ATTRIBUTES);
}

namespace Fsys {

	namespace Directory {
		bool is_exist(PCWSTR path) {
			return Fsys::is_exist(path) && Fsys::is_dir(path);
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
			} catch (Ext::AbstractError & e) {
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

		void del(PCWSTR path) {
			CheckApi(del_nt(path));
		}
	}

}
