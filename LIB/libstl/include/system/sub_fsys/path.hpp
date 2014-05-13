#ifndef LIBSTL_SYSTEM_FSYS_PATH_HPP_
#define LIBSTL_SYSTEM_FSYS_PATH_HPP_

#include <system/fsys.hpp>

#include <simstd/iosfwd>
#include <simstd/string>

namespace fsys {
	namespace Filename {
		bool is_valid(const wchar_t * name);
		inline bool is_valid(const ustring & name)
		{
			return is_valid(name.c_str());
		}

		ustring extract(const ustring & path);

	}

	namespace Path {
		namespace inplace {
			ustring & remove_prefix(ustring & path, const ustring & pref = ustring(PATH_PREFIX_NT));

			ustring & ensure_prefix(ustring & path, const ustring & pref = ustring(PATH_PREFIX_NT));

			ustring & ensure_end_separator(ustring & path, wchar_t sep = PATH_SEPARATOR_C);

			ustring & ensure_no_end_separator(ustring & path);

		}

		inline ustring remove_prefix(const ustring & path, const ustring & pref = ustring(PATH_PREFIX_NT))
		{
			ustring tmp(path);
			return inplace::remove_prefix(tmp, pref);
		}

		inline ustring ensure_prefix(const ustring & path, const ustring & pref = ustring(PATH_PREFIX_NT))
		{
			ustring tmp(path);
			return inplace::ensure_prefix(tmp, pref);
		}

		inline ustring ensure_end_separator(const ustring & path, wchar_t sep = PATH_SEPARATOR_C)
		{
			ustring tmp(path);
			return inplace::ensure_end_separator(tmp, sep);
		}

		inline ustring ensure_no_end_separator(const ustring & path)
		{
			ustring tmp(path);
			return inplace::ensure_no_end_separator(tmp);
		}

		ustring get_root(const wchar_t * path);
		inline ustring get_root(const ustring & path)
		{
			return get_root(path.c_str());
		}

		ustring get_work_directory();

		bool set_work_directory(const wchar_t * path);
		inline bool set_work_directory(const ustring & path)
		{
			return set_work_directory(path.c_str());
		}

		bool is_relative(const wchar_t * path);
		inline bool is_relative(const ustring & path)
		{
			return is_relative(path.c_str());
		}

		bool is_mask(const wchar_t * path);
		inline bool is_mask(const ustring & path)
		{
			return is_mask(path.c_str());
		}

		bool expand(wchar_t * dest, size_t length, const wchar_t * path);
		ustring expand(const wchar_t * path);
		inline ustring expand(const ustring & path)
		{
			return expand(path.c_str());
		}

		bool unexpand(wchar_t * dest, size_t length, const wchar_t * path);
		ustring unexpand(const wchar_t * path);
		inline ustring unexpand(const ustring & path)
		{
			return unexpand(path.c_str());
		}

		bool compact(wchar_t * dest, size_t length, const wchar_t * path);
		ustring compact(const wchar_t * path, size_t size);
		inline ustring compact(const ustring & path, size_t size)
		{
			return compact(path.c_str(), size);
		}

		ustring extract_from_mask(const ustring & path);

		ustring extract(const ustring & path);

		ustring get_special(int csidl, bool create = true);

		ustring get_fullpath(const wchar_t * path);
		inline ustring get_fullpath(const ustring &path)
		{
			return get_fullpath(path.c_str());
		}

		bool canonicalize(wchar_t * dest, const wchar_t * path);
		ustring canonicalize(const wchar_t * path);
		inline ustring canonicalize(const ustring &path)
		{
			return canonicalize(path.c_str());
		}

		bool is_mask_match(const wchar_t * path, const wchar_t * mask, DWORD flags = 0);

	}

}

namespace fsys {

	ustring PathNice(const wchar_t * path);
	inline ustring PathNice(const ustring &path)
	{
		return PathNice(path.c_str());
	}

	ustring Secure(const wchar_t * path);
	inline ustring Secure(const ustring &path)
	{
		return Secure(path.c_str());
	}

	ustring Validate(const wchar_t * path);
	inline ustring Validate(const ustring &path)
	{
		return Validate(path.c_str());
	}

	ustring MakePath(const wchar_t * path, const wchar_t * name);
	inline ustring MakePath(const ustring & path, const ustring &name)
	{
		return MakePath(path.c_str(), name.c_str());
	}

	ustring TempDir();

	ustring TempFile(const wchar_t * path);
	inline ustring TempFile(const ustring & path)
	{
		return TempFile(path.c_str());
	}

	bool substr_match(const ustring& str, size_t pos, const wchar_t * mstr);

	void locate_path_root(const ustring& path, size_t& path_root_len, bool& is_unc_path);

	ustring extract_file_name(const ustring& path);

	///===================================================================================== SysPath
	namespace SysPath {
		ustring Winnt();
		ustring Sys32();
		ustring SysNative();
		ustring InetSrv();
		ustring Dns();
		ustring Temp();

		ustring Users();
	}

	///====================================================================================== SysApp
	namespace SysApp {
		ustring appcmd();
	//	ustring dnscmd();
	}

}

#endif
