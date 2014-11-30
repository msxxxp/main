#include <basis/configure.hpp>

const wchar_t* funcname_only(const wchar_t* path, wchar_t ch)
{
	const wchar_t* tmp = ::wcschr((path), ch);
	if (tmp) {
		static wchar_t str[128];
		::wcsncpy(str, path, tmp - path);
		str[tmp - path] = L'\0';
		return str;
	}
	return path;
}
