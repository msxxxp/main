#include <basis/os/shlwapi.hpp>

namespace os {

	shlwapi_dll & shlwapi_dll::inst()
	{
		static shlwapi_dll ret;
		return ret;
	}

	shlwapi_dll::shlwapi_dll() :
		DynamicLibrary(L"shlwapi.dll")
	{
		GET_DLL_FUNC(PathMatchSpecExW);
		GET_DLL_FUNC(PathCanonicalizeW);
		GET_DLL_FUNC(PathCompactPathExW);
		GET_DLL_FUNC(PathIsDirectoryEmptyW);
		GET_DLL_FUNC(PathUnExpandEnvStringsW);
		GET_DLL_FUNC(PathIsRelativeW);
	}

}
