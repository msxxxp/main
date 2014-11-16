#include <basis/configure.hpp>

const char* filename_only(const char* path)
{
	const char* tmp = ::strrchr(path, '\\');
	if (tmp == nullptr)
		tmp = ::strrchr(path, '/');
	return (tmp ? tmp : path - 1) + 1;
//	return (strrchr((path), ch) ? : (path) - 1) + 1;
}
