#include <basis/sys/fsys.hpp>
#include <basis/sys/linkage.hpp>

namespace fsys {

	namespace {
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

			static shlwapi_dll & inst()
			{
				static shlwapi_dll ret;
				return ret;
			}

		private:
			shlwapi_dll() :
				DynamicLibrary(L"shlwapi.dll")
			{
				GET_DLL_FUNC(PathMatchSpecExW);
				GET_DLL_FUNC(PathCanonicalizeW);
				GET_DLL_FUNC(PathCompactPathExW);
				GET_DLL_FUNC(PathIsDirectoryEmptyW);
				GET_DLL_FUNC(PathUnExpandEnvStringsW);
				GET_DLL_FUNC(PathIsRelativeW);
			}
		};
	}

	namespace Path {
		bool is_relative(const wchar_t * path)
		{
			return shlwapi_dll::inst().PathIsRelativeW(path);
		}

		bool canonicalize(wchar_t * dest, const wchar_t * path)
		{
			return shlwapi_dll::inst().PathCanonicalizeW(dest, path);
		}

		bool expand(wchar_t * dest, size_t length, const wchar_t * path)
		{
			return ::ExpandEnvironmentStringsW(path, dest, length);
		}

		bool unexpand(wchar_t * dest, size_t length, const wchar_t * path)
		{
			return shlwapi_dll::inst().PathUnExpandEnvStringsW(path, dest, length);
		}

		bool compact(wchar_t * dest, size_t length, const wchar_t * path)
		{
			return shlwapi_dll::inst().PathCompactPathExW(dest, path, length, 0);
		}

		bool is_mask_match(const wchar_t * path, const wchar_t * mask, DWORD flags)
		{
			return shlwapi_dll::inst().PathMatchSpecExW(path, mask, flags) == S_OK ;
		}

	}

	namespace Directory {
		bool is_empty(const wchar_t * path)
		{
			return shlwapi_dll::inst().PathIsDirectoryEmptyW(path);
		}
	}

}
