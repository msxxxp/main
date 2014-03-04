#ifndef WIN_NET_POLICY_HPP
#define WIN_NET_POLICY_HPP

#include <system/configure.hpp>
#include <system/string.hpp>

#include <ntsecapi.h>

namespace Ext {

	namespace Policy {
		LSA_UNICODE_STRING ToLsaString(PCWSTR in);

		LSA_HANDLE GetHandle(PCWSTR dom = EMPTY_STR);

		void AccountRightAdd(PCWSTR right, PCWSTR name, PCWSTR dom = EMPTY_STR);

		void AccountRightDel(PCWSTR right, PCWSTR name, PCWSTR dom = EMPTY_STR);

		bool GetTokenUser(HANDLE hToken, ustring & name);
	}

}

#endif
