#include <basis/sys/url.hpp>
#include <basis/sys/memory.hpp>
#include <basis/os/shlwapi.hpp>
#include <basis/os/wininet.hpp>

namespace Url {

	HRESULT parse(const wchar_t * url, PARSEDURL * parsedUrl)
	{
		return os::shlwapi_dll::inst().ParseURLW(url, parsedUrl);
	}

	bool crack(const wchar_t * url, URL_COMPONENTSW * info)
	{
		memory::zero(*info);
		info->dwStructSize = sizeof(*info);

		// Set required component lengths to non-zero so that they are cracked.
		info->dwSchemeLength = (DWORD)-1;
		info->dwHostNameLength = (DWORD)-1;
		info->dwUrlPathLength = (DWORD)-1;
		info->dwExtraInfoLength = (DWORD)-1;

		return os::wininet_dll::inst().InternetCrackUrlW(url, 0, 0, info);
	}

}
