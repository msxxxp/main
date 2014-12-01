#ifndef WIN_NET_POLICY_HPP
#define WIN_NET_POLICY_HPP

#include <basis/sys/sstr.hpp>

#include <ntsecapi.h>

namespace Ext {

	namespace Policy {
		LSA_UNICODE_STRING ToLsaString(const wchar_t* in);

		LSA_HANDLE GetHandle(const wchar_t* dom = EMPTY_STR);

		void AccountRightAdd(const wchar_t* right, const wchar_t* name, const wchar_t* dom = EMPTY_STR);

		void AccountRightDel(const wchar_t* right, const wchar_t* name, const wchar_t* dom = EMPTY_STR);

		bool GetTokenUser(HANDLE hToken, ustring& name);
	}

}

#endif
