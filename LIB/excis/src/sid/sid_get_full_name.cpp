﻿#include <excis/sid.hpp>
#include <excis/exception.hpp>

#include <basis/simstd/string>

namespace Ext {

	ustring Sid::get_full_name() const {
		return this_type::get_full_name(m_sid);
	}

	ustring Sid::get_full_name(value_type sid, PCWSTR srv) {
		ustring nam, dom;
		get_name_dom(sid, nam, dom, srv);
		if (!dom.empty() && !nam.empty()) {
			dom.reserve(dom.size() + nam.size() + 1);
			dom += PATH_SEPARATOR;
			dom += nam;
			return dom;
		}
		return nam;
	}

}
