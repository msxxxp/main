#include <basis/sys/url.hpp>
#include <basis/os/shlwapi.hpp>

namespace url {

	HRESULT parse(const wchar_t * url, PARSEDURL * parsedUrl)
	{
		return os::shlwapi_dll::inst().ParseURLW(url, parsedUrl);
	}

}
