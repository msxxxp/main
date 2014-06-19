#ifndef BASIS_SYS_CONFIGURE_OBLIGATORY_THIS_FILE_HPP_
#define BASIS_SYS_CONFIGURE_OBLIGATORY_THIS_FILE_HPP_

inline const char * filename_only(const char * path, char ch = '\\')
{
	const char * tmp = ::strrchr((path), ch);
	return (tmp ? tmp : (path) - 1) + 1;
//		return (strrchr((path), ch) ? : (path) - 1) + 1;
}

inline const wchar_t * filename_only(const wchar_t * path, wchar_t ch = PATH_SEPARATOR_C)
{
	const wchar_t * tmp = ::wcsrchr((path), ch);
	return (tmp ? tmp : (path) - 1) + 1;
//		return (wcsrchr((path), ch) ? : (path) - 1) + 1;
}

#endif
