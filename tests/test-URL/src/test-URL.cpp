#include <system/console.hpp>
#include <system/cstr.hpp>
#include <system/url.hpp>

#include <liblog/logger.hpp>

#include <simstd/string>

#include <Wininet.h>

void sample(PCTSTR pcszUrl)
{
	PARSEDURL pu;
	pu.cbSize = sizeof(pu);

	HRESULT hr = Url::parse(pcszUrl, &pu);
	console::printf(L"ParseURL(%s) returned 0x%08x\n", pcszUrl, hr);

	if (SUCCEEDED(hr)) {
		console::printf(L"Protocol = %.*s\n", pu.cchProtocol, pu.pszProtocol);
		console::printf(L"Size     = %u\n", pu.cbSize);
		console::printf(L"Suffix   = %.*s\n", pu.cchSuffix, pu.pszSuffix);
		console::printf(L"Scheme   = %d\n", pu.nScheme);
		console::printf(L"\n");
	}
}

void sample2(PCWSTR url)
{
	URL_COMPONENTSW info;

	bool res = Url::crack(url, &info);

	if (res) {
		console::printf(L"Scheme   = '%.*s'\n", info.dwSchemeLength, info.lpszScheme);
		console::printf(L"Scheme   = %d\n", info.nScheme);
		console::printf(L"Host     = '%.*s'\n", info.dwHostNameLength, info.lpszHostName);
		console::printf(L"Port     = %d\n", info.nPort);
		console::printf(L"User     = '%.*s'\n", info.dwUserNameLength, info.lpszUserName);
		console::printf(L"Pass     = '%.*s'\n", info.dwPasswordLength, info.lpszPassword);
		console::printf(L"Path     = '%.*s'\n", info.dwUrlPathLength, info.lpszUrlPath);
		console::printf(L"Extr     = '%.*s'\n", info.dwExtraInfoLength, info.lpszExtraInfo);
		console::printf(L"\n");
	} else {
		console::printf(L"Error: %u\n", GetLastError());
	}
}

int main()
{
//	sample2(L"http://msdn.microsoft.com/vstudio/");
//	sample2(L"mailto:someone@example.com");
//	sample2(L"file://C:\\AUTOEXEC.BAT");
//	sample2(L"logger:///default?color=true;level=debug;prefix=full;target=console");
//	sample2(L"logger:///module?name=default;color=true;level=debug;prefix=full;target=console");
//	set_options(L"logger:///module");
//	set_options(L"logger:///module?name=default;color=true;level=debug;prefix=full;target=console");
//	set_options(L"logger:///default?color=true;level=debug;prefix=full;target=file(c:\\);");
	LogOptions(L"logger:///module?name=default;color=true;level=debug;;;prefix=full;target=console;;;");

//	LogOptions(L"logger:///default?level=i;prefix=fu;target=fo(c:\\);");
	return 0;
}
