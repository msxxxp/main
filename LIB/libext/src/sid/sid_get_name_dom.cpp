#include <libext/sid.hpp>
#include <libext/exception.hpp>
#include <basis/sys/memory.hpp>

#include <basis/std/string>

namespace Ext {

	void Sid::get_name_dom(value_type sid, ustring & name, ustring & dom, PCWSTR srv) {
		check(sid);
		DWORD size_nam = 0;
		DWORD size_dom = 0;
		SID_NAME_USE type;

		// determine size of name
		::LookupAccountSidW(srv, sid, nullptr, &size_nam, nullptr, &size_dom, &type);
		CheckApi(::GetLastError() == ERROR_INSUFFICIENT_BUFFER);
		memory::auto_array<wchar_t> pName(size_nam);
		memory::auto_array<wchar_t> pDom(size_dom);

		CheckApi(::LookupAccountSidW(srv, sid, pName.data(), &size_nam, pDom.data(), &size_dom, &type));
		name = pName;
		dom = pDom;
	}

}
