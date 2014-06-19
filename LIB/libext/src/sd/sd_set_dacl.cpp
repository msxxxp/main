#include <libext/sd.hpp>
#include <libext/exception.hpp>
#include <basis/ext/bits.hpp>

namespace Ext {

	void set_dacl(PCWSTR path, PSECURITY_DESCRIPTOR sd, SE_OBJECT_TYPE type) {
		WORD control = WinSD::get_control(sd);
		if (bits::Flags::check(control, (WORD)SE_DACL_PRESENT)) {
			DWORD flag = (bits::Flags::check(control, (WORD)SE_DACL_PROTECTED)) ?
				PROTECTED_DACL_SECURITY_INFORMATION
				:
				UNPROTECTED_DACL_SECURITY_INFORMATION;
			CheckApiError(::SetNamedSecurityInfoW((PWSTR)path, type,
			                                      DACL_SECURITY_INFORMATION | flag, nullptr, nullptr,
			                                      WinSD::get_dacl(sd), nullptr));
		}
	}

}
