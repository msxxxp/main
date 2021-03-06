#ifndef BASIS_SYS_FSYS_ATTRIBUTES_HPP_
#define BASIS_SYS_FSYS_ATTRIBUTES_HPP_

namespace fsys {

	typedef uint32_t Attributes;

	Attributes get_attr_nt(const wchar_t* path);
	bool set_attr_nt(const wchar_t* path, Attributes attr);

	bool is_file(Attributes attr);
	bool is_dir(Attributes attr);
	bool is_link(Attributes attr);

	bool is_file_nt(const wchar_t* path);
	bool is_dir_nt(const wchar_t* path);
	bool is_link_nt(const wchar_t* path);

}

namespace fsys {

	inline bool is_valid(Attributes attr)
	{
		return attr != INVALID_FILE_ATTRIBUTES;
	}

	inline bool is_file(Attributes attr)
	{
		return is_valid(attr) && !(attr & FILE_ATTRIBUTE_DIRECTORY);
	}

	inline bool is_dir(Attributes attr)
	{
		return is_valid(attr) && (attr & FILE_ATTRIBUTE_DIRECTORY);
	}

	inline bool is_link(Attributes attr)
	{
		return is_valid(attr) && (attr & FILE_ATTRIBUTE_REPARSE_POINT);
	}

}

#endif
