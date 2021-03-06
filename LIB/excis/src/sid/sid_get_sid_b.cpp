﻿#include <excis/sid.hpp>
#include <excis/exception.hpp>
#include <basis/sys/memory.hpp>

namespace Ext {

	PSID Sid::get_sid(PCWSTR name, PCWSTR srv) {
		DWORD size_sid = SECURITY_MAX_SID_SIZE;
		PSID m_sid = (value_type)::LocalAlloc(LPTR, size_sid);
		DWORD size_dom = MAX_PATH;
		memory::auto_array<wchar_t> dom(size_dom);
		SID_NAME_USE type;
		CheckApi(::LookupAccountNameW(srv, name, m_sid, &size_sid, dom.data(), &size_dom, &type));
		return m_sid;
	}

}
