#include <basis/configure.hpp>

const wchar_t* filename_only(const wchar_t* path)
{
	const wchar_t* tmp = ::wcsrchr(path, L'\\');
	if (tmp == nullptr)
		tmp = ::wcsrchr(path, L'/');
	return (tmp ? tmp : path - 1) + 1;
//	return (wcsrchr((path), ch) ? : (path) - 1) + 1;
}
