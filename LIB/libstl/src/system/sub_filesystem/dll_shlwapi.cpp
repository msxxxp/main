#include <system/fsys.hpp>
#include <system/linkage.hpp>

namespace fsys {

	namespace {
		struct shlwapi_dll: private linkage::DynamicLibrary {
			typedef HRESULT (*FPathMatchSpecExW)(PCWSTR, PCWSTR, DWORD);
			typedef WINBOOL (*FPathCanonicalizeW)(PWSTR, PCWSTR);
			typedef WINBOOL (*FPathCompactPathExW)(PWSTR, PCWSTR, UINT, DWORD);
			typedef WINBOOL (*FPathIsDirectoryEmptyW)(PCWSTR);
			typedef WINBOOL (*FPathUnExpandEnvStringsW)(PCWSTR, PWSTR, UINT);
			typedef WINBOOL (*FPathIsRelativeW)(PCWSTR);

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
		bool is_relative(PCWSTR path)
		{
			return shlwapi_dll::inst().PathIsRelativeW(path);
		}

		bool canonicalize(PWSTR dest, PCWSTR path)
		{
			return shlwapi_dll::inst().PathCanonicalizeW(dest, path);
		}

		bool expand(PWSTR dest, size_t length, PCWSTR path)
		{
			return ::ExpandEnvironmentStringsW(path, dest, length);
		}

		bool unexpand(PWSTR dest, size_t length, PCWSTR path)
		{
			return shlwapi_dll::inst().PathUnExpandEnvStringsW(path, dest, length);
		}

		bool compact(PWSTR dest, size_t length, PCWSTR path)
		{
			return shlwapi_dll::inst().PathCompactPathExW(dest, path, length, 0);
		}

		bool is_mask_match(PCWSTR path, PCWSTR mask, DWORD flags)
		{
			return shlwapi_dll::inst().PathMatchSpecExW(path, mask, flags) == S_OK ;
		}

	}

	namespace Directory {
		bool is_empty(PCWSTR path)
		{
			return shlwapi_dll::inst().PathIsDirectoryEmptyW(path);
		}
	}

}
