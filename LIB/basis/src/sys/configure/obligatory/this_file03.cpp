#include <basis/configure.hpp>

const char* funcname_only(const char* path, char ch)
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
