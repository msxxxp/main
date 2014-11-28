#ifndef LIBBASE_STD_HPP_
#define LIBBASE_STD_HPP_

#include <basis/configure.hpp>

typedef const void * PCVOID;

namespace Base {

	const uint32_t BOM_UTF32le = 0x0000FEFF;
	const uint32_t BOM_UTF32be = 0xFFFE0000;
	const uint32_t BOM_UTF16le = 0xFEFF;
	const uint32_t BOM_UTF16be = 0xFFFE;
	const uint32_t BOM_UTF8 = 0xBFBBEF00;

	const UINT CP_UTF16le = 1200;
	const UINT CP_UTF16be = 1201;
	const UINT CP_UTF32le = 1202;
	const UINT CP_UTF32be = 1203;
//	const size_t CP_AUTODETECT = (UINT)-1;
	const UINT DEFAULT_CP = CP_UTF8;

}

#endif
