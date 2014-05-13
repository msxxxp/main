﻿#ifndef LIBSTL_SYSTEM_URL_HPP_
#define LIBSTL_SYSTEM_URL_HPP_

#include <system/configure.hpp>

#include <Wininet.h>

typedef struct tagPARSEDURL {
	DWORD cbSize;
	PCWSTR pszProtocol;
	UINT cchProtocol;
	PCWSTR pszSuffix;
	UINT cchSuffix;
	UINT nScheme;
} PARSEDURL, *PPARSEDURL;

namespace Url {

	HRESULT parse(const wchar_t * url, PARSEDURL * parsedUrl);

	bool crack(const wchar_t * url, URL_COMPONENTSW * info);

}

#endif