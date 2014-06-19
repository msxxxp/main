#include <basis/sys/memory.hpp>
#include <excis/fileversion.hpp>
#include <excis/dll.hpp>

#include <excis/exception.hpp>

namespace Ext {

	namespace {
		struct version_dll: private DynamicLibrary {
			typedef DWORD (WINAPI *FGetFileVersionInfoSizeW)(LPCWSTR, LPDWORD);
			typedef WINBOOL (WINAPI *FGetFileVersionInfoW)(LPCWSTR, DWORD, DWORD, LPVOID);
			typedef WINBOOL (WINAPI *FVerQueryValueW)(const LPVOID, LPCWSTR, LPVOID *, PUINT);

			DEFINE_FUNC(GetFileVersionInfoSizeW);
			DEFINE_FUNC(GetFileVersionInfoW);
			DEFINE_FUNC(VerQueryValueW);

			static version_dll & inst() {
				static version_dll ret;
				return ret;
			}

		private:
			version_dll():
				DynamicLibrary(L"version.dll") {
				GET_DLL_FUNC(GetFileVersionInfoSizeW);
				GET_DLL_FUNC(GetFileVersionInfoW);
				GET_DLL_FUNC(VerQueryValueW);
			}
		};
	}

	///================================================================================= FileVersion
	FileVersion::FileVersion(PCWSTR path) {
		DWORD size = version_dll::inst().GetFileVersionInfoSizeW(path, nullptr);
		CheckApi(size);
		memory::auto_array<BYTE> data(size);
		CheckApi(version_dll::inst().GetFileVersionInfoW(path, 0, size, data.data()));
		UINT bufLen;
		VS_FIXEDFILEINFO * ffi;
		CheckApi(version_dll::inst().VerQueryValueW(data, L"\\", (PVOID*)&ffi, &bufLen));
		wchar_t tmp[MAX_PATH];
		_snwprintf(tmp, lengthof(tmp), L"%d.%d"
		           ,HIWORD(ffi->dwFileVersionMS)
		           ,LOWORD(ffi->dwFileVersionMS)
		           //	           ,HIWORD(ffi->dwFileVersionLS)
		           //	           LOWORD(ffi->dwFileVersionLS)
		);
		m_ver = tmp;
	}
}
