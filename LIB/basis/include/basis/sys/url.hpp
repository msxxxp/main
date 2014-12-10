#ifndef BASIS_SYS_URL_HPP_
#define BASIS_SYS_URL_HPP_

#include <basis/configure.hpp>

#include <wininet.h>

typedef struct tagPARSEDURL {
	DWORD cbSize;
	PCWSTR pszProtocol;
	UINT cchProtocol;
	PCWSTR pszSuffix;
	UINT cchSuffix;
	UINT nScheme;
} PARSEDURL, *PPARSEDURL;

namespace url {

	HRESULT parse(const wchar_t* url, PARSEDURL * parsedUrl);

	bool crack(const wchar_t* url, URL_COMPONENTSW * info);

}

#endif
