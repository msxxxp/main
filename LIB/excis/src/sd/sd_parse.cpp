#include <basis/ext/bits.hpp>
//#include <libbase/bit_str.hpp>
#include <basis/sys/logger.hpp>
#include <basis/simstd/string>

#include <excis/sd.hpp>
#include <excis/exception.hpp>
#include <excis/dacl.hpp>


namespace Ext {

	void WinSD::parse(PSECURITY_DESCRIPTOR sd) {
#ifdef ENABLE_LOGGER
		WORD ctrl = get_control(sd);
		LogInfo(L"Security descriptor:\n");
		LogInfo(L"SDDL: %s\n", WinSD::as_sddl(sd).c_str());
		LogInfo(L"Size: %Iu\n", size(sd));
		try {
			LogInfo(L"Owner: %s\n", Sid::get_name(get_owner(sd)).c_str());
		} catch (...) {
		}
		try {
			LogInfo(L"Group: %s\n", Sid::get_name(get_group(sd)).c_str());
		} catch (...) {
		}
		LogInfo(L"Control: 0x%x (%s) [%s]\n", (int)ctrl, bits::BitMask<WORD>::to_str_bin(ctrl).c_str(), bits::BitMask<WORD>::to_str_num(ctrl).c_str());
		if (bits::Flags::check(ctrl, (WORD)SE_OWNER_DEFAULTED))
			LogInfo(L"\tSE_OWNER_DEFAULTED (%s)\n", simstd::to_wstring(SE_OWNER_DEFAULTED).c_str());
		if (bits::Flags::check(ctrl, (WORD)SE_GROUP_DEFAULTED))
			LogInfo(L"\tSE_GROUP_DEFAULTED (%s)\n", simstd::to_wstring(SE_GROUP_DEFAULTED).c_str());
		if (bits::Flags::check(ctrl, (WORD)SE_DACL_PRESENT))
			LogInfo(L"\tSE_DACL_PRESENT (%s)\n", simstd::to_wstring(SE_DACL_PRESENT).c_str());
		if (bits::Flags::check(ctrl, (WORD)SE_DACL_DEFAULTED))
			LogInfo(L"\tSE_DACL_DEFAULTED (%s)\n", simstd::to_wstring(SE_DACL_DEFAULTED).c_str());
		if (bits::Flags::check(ctrl, (WORD)SE_SACL_PRESENT))
			LogInfo(L"\tSE_SACL_PRESENT (%s)\n", simstd::to_wstring(SE_SACL_PRESENT).c_str());
		if (bits::Flags::check(ctrl, (WORD)SE_SACL_DEFAULTED))
			LogInfo(L"\tSE_SACL_DEFAULTED (%s)\n", simstd::to_wstring(SE_SACL_DEFAULTED).c_str());
		if (bits::Flags::check(ctrl, (WORD)SE_DACL_AUTO_INHERIT_REQ))
			LogInfo(L"\tSE_DACL_AUTO_INHERIT_REQ (%s)\n", simstd::to_wstring(SE_DACL_AUTO_INHERIT_REQ).c_str());
		if (bits::Flags::check(ctrl, (WORD)SE_SACL_AUTO_INHERIT_REQ))
			LogInfo(L"\tSE_SACL_AUTO_INHERIT_REQ (%s)\n", simstd::to_wstring(SE_SACL_AUTO_INHERIT_REQ).c_str());
		if (bits::Flags::check(ctrl, (WORD)SE_DACL_AUTO_INHERITED))
			LogInfo(L"\tSE_DACL_AUTO_INHERITED (%s)\n", simstd::to_wstring(SE_DACL_AUTO_INHERITED).c_str());
		if (bits::Flags::check(ctrl, (WORD)SE_SACL_AUTO_INHERITED))
			LogInfo(L"\tSE_SACL_AUTO_INHERITED (%s)\n", simstd::to_wstring(SE_SACL_AUTO_INHERITED).c_str());
		if (bits::Flags::check(ctrl, (WORD)SE_DACL_PROTECTED))
			LogInfo(L"\tSE_DACL_PROTECTED (%s)\n", simstd::to_wstring(SE_DACL_PROTECTED).c_str());
		if (bits::Flags::check(ctrl, (WORD)SE_SACL_PROTECTED))
			LogInfo(L"\tSE_SACL_PROTECTED (%s)\n", simstd::to_wstring(SE_SACL_PROTECTED).c_str());
		if (bits::Flags::check(ctrl, (WORD)SE_SELF_RELATIVE))
			LogInfo(L"\tSE_SELF_RELATIVE (%s)\n", simstd::to_wstring(SE_SELF_RELATIVE).c_str());
#else
		(void)sd;
#endif
	}
}
