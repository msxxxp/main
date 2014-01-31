#ifndef LIBSTL_SYSTEM_CONFIGURE_OBLIGATORY_DEFINITIONS_HPP_
#define LIBSTL_SYSTEM_CONFIGURE_OBLIGATORY_DEFINITIONS_HPP_

#define UNUSED(val) ((void)val)

#define THIS_FILE        filename_only(__FILE__)
#define THIS_PLACE       THIS_FILE, __LINE__, __PRETTY_FUNCTION__
#define THIS_PLACE_SHORT THIS_FILE, __LINE__, __FUNCTION__

inline void mbox(PCSTR text, PCSTR capt = "")
{
	::MessageBoxA(nullptr, text, capt, MB_OK);
}

inline void mbox(PCWSTR text, PCWSTR capt = L"")
{
	::MessageBoxW(nullptr, text, capt, MB_OK);
}

#endif
