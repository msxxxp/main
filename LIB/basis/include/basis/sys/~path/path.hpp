#ifndef BASIS_SYS_PATH_PATH_HPP_
#define BASIS_SYS_PATH_PATH_HPP_

namespace path {

	bool is_relative(const wchar_t* path);
	bool is_relative(const ustring& path);

	bool canonicalize(wchar_t* dest, const wchar_t* path);
	ustring canonicalize(const wchar_t* path);
	ustring canonicalize(const ustring& path);

	bool expand(wchar_t* dest, size_t length, const wchar_t* path);
	ustring expand(const wchar_t* path);
	ustring expand(const ustring& path);

	bool unexpand(wchar_t* dest, size_t length, const wchar_t* path);
	ustring unexpand(const wchar_t* path);
	ustring unexpand(const ustring& path);

	bool compact(wchar_t* dest, size_t length, const wchar_t* path);
	ustring compact(const wchar_t* path, size_t size);
	ustring compact(const ustring& path, size_t size);

	bool is_mask_match(const wchar_t* path, const wchar_t* mask, DWORD flags = 0);

	ustring get_work_directory();

	bool set_work_directory(const wchar_t* path);
	bool set_work_directory(const ustring& path);

	ustring get_root(const wchar_t* path);
	ustring get_root(const ustring& path);

	bool is_mask(const wchar_t* path);
	bool is_mask(const ustring& path);

	ustring extract_from_mask(const ustring& path);

	ustring make(const wchar_t* path, const wchar_t* name);
	ustring make(const ustring& path, const ustring& name);

	ustring extract(const ustring& path);

	ustring get_special(int csidl, bool create = true);

	ustring get_fullpath(const wchar_t* path);
	ustring get_fullpath(const ustring& path);

	ustring nice(const wchar_t* path);
	ustring nice(const ustring& path);

	ustring secure(const wchar_t* path);
	ustring secure(const ustring& path);

	ustring validate(const wchar_t* path);
	ustring validate(const ustring& path);

	ustring temp_dir();

	ustring temp_file(const wchar_t* path);
	ustring temp_file(const ustring& path);

}

#endif
