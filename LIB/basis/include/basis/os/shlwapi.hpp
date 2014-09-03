#ifndef BASIS_OS_SHLWAPI_
#define BASIS_OS_SHLWAPI_

#include <basis/sys/linkage.hpp>

namespace os {

	struct shlwapi_dll: private linkage::DynamicLibrary {
		typedef HRESULT WINAPI (*FPathMatchSpecExW)(const wchar_t *, const wchar_t *, DWORD);
		typedef WINBOOL WINAPI (*FPathCanonicalizeW)(wchar_t *, const wchar_t *);
		typedef WINBOOL WINAPI (*FPathCompactPathExW)(wchar_t *, const wchar_t *, UINT, DWORD);
		typedef WINBOOL WINAPI (*FPathIsDirectoryEmptyW)(const wchar_t *);
		typedef WINBOOL WINAPI (*FPathUnExpandEnvStringsW)(const wchar_t *, wchar_t *, UINT);
		typedef WINBOOL WINAPI (*FPathIsRelativeW)(const wchar_t *);

		DEFINE_FUNC(PathMatchSpecExW);
		DEFINE_FUNC(PathCanonicalizeW);
		DEFINE_FUNC(PathCompactPathExW);
		DEFINE_FUNC(PathIsDirectoryEmptyW);
		DEFINE_FUNC(PathUnExpandEnvStringsW);
		DEFINE_FUNC(PathIsRelativeW);

		static shlwapi_dll & inst();

	private:
		shlwapi_dll();
	};

}

#endif
