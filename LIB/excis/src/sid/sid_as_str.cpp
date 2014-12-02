#include <excis/sid.hpp>
#include <excis/exception.hpp>
#include <basis/sys/memory.hpp>
#include <basis/simstd/string>

extern "C" {
	WINADVAPI BOOL WINAPI ConvertSidToStringSidW(PSID Sid, PWSTR * StringSid);
}


namespace Ext {

	ustring Sid::as_str() const {
		return this_type::as_str(m_sid);
	}

	// PSID to sid string
	ustring Sid::as_str(value_type psid) {
		check(psid);
		memory::auto_close<PWSTR> ret(nullptr, ::LocalFree);
		CheckApi(::ConvertSidToStringSidW(psid, &ret));
		return ustring(ret);
	}

}
