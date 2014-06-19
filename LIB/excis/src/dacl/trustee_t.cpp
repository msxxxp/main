#include <excis/dacl.hpp>
#include <excis/exception.hpp>

namespace Ext {

	///=================================================================================== trustee_t
	trustee_t::trustee_t(PCWSTR name) {
		::BuildTrusteeWithNameW(this, (PWSTR)name);
	}

	trustee_t::trustee_t(PSID sid) {
		::BuildTrusteeWithSidW(this, sid);
	}

}
