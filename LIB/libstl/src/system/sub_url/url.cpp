#include <system/url.hpp>
#include <system/linkage.hpp>

namespace {

	struct shlwapi_dll: private linkage::DynamicLibrary {
		typedef HRESULT (*FParseURLW)(const wchar_t *, PARSEDURL *);

		DEFINE_FUNC(ParseURLW);

		static shlwapi_dll & inst()
		{
			static shlwapi_dll ret;
			return ret;
		}

	private:
		shlwapi_dll() :
			DynamicLibrary(L"shlwapi.dll")
		{
			GET_DLL_FUNC(ParseURLW);
		}
	};

	struct wininet_dll: private linkage::DynamicLibrary {
		typedef BOOL (*FInternetCrackUrlW)(const wchar_t *, DWORD dwUrlLength, DWORD dwFlags, LPURL_COMPONENTS lpUrlComponents);

		DEFINE_FUNC(InternetCrackUrlW);

		static wininet_dll & inst()
		{
			static wininet_dll ret;
			return ret;
		}

	private:
		wininet_dll() :
			DynamicLibrary(L"wininet.dll")
		{
			GET_DLL_FUNC(InternetCrackUrlW);
		}
	};

}

namespace Url {

	HRESULT parse(const wchar_t * url, PARSEDURL * parsedUrl)
	{
		return shlwapi_dll::inst().ParseURLW(url, parsedUrl);
	}

	bool crack(const wchar_t * url, URL_COMPONENTSW * info)
	{
		::ZeroMemory(info, sizeof(*info));
		info->dwStructSize = sizeof(*info);

		// Set required component lengths to non-zero so that they are cracked.
		info->dwSchemeLength = (DWORD)-1;
		info->dwHostNameLength = (DWORD)-1;
		info->dwUrlPathLength = (DWORD)-1;
		info->dwExtraInfoLength = (DWORD)-1;

		return wininet_dll::inst().InternetCrackUrlW(url, 0, 0, info);
	}

}
