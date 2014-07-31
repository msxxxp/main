#ifndef BASIS_SYS_CONFIGURE_OBLIGATORY_THIS_FILE_HPP_
#define BASIS_SYS_CONFIGURE_OBLIGATORY_THIS_FILE_HPP_

inline const char * filename_only(const char * path, char ch = '\\')
{
	const char * tmp = ::strrchr(path, ch);
	return (tmp ? tmp : (path) - 1) + 1;
//	return (strrchr((path), ch) ? : (path) - 1) + 1;
}

inline const wchar_t * filename_only(const wchar_t * path, wchar_t ch = PATH_SEPARATOR_C)
{
	const wchar_t * tmp = ::wcsrchr(path, ch);
	return (tmp ? tmp : (path) - 1) + 1;
//	return (wcsrchr((path), ch) ? : (path) - 1) + 1;
}

inline const char * funcname_only(const char * path, char ch = '(')
{
	const char * tmp = ::strchr(path, ch);
	if (tmp) {
		static char str[128];
		::strncpy(str, path, tmp - path);
		str[tmp - path] = '\0';
		return str;
	}
	return path;
}

inline const wchar_t * funcname_only(const wchar_t * path, wchar_t ch = L'(')
{
	const wchar_t * tmp = ::wcschr((path), ch);
	if (tmp) {
		static wchar_t str[128];
		::wcsncpy(str, path, tmp - path);
		str[tmp - path] = L'\0';
		return str;
	}
	return path;
}

#endif
