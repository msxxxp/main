#ifndef HDLINK_CRYPTPROVIDER_HPP_
#define HDLINK_CRYPTPROVIDER_HPP_

#include <libbase/std.hpp>

#include <wincrypt.h>

struct CryptProvider {
	~CryptProvider();

	// type = (PROV_RSA_FULL, PROV_RSA_AES)
	CryptProvider(PCWSTR prov = nullptr, DWORD type = PROV_RSA_FULL);

	operator HCRYPTPROV() const
	{
		return m_hnd;
	}

private:
	HCRYPTPROV m_hnd;
};

#endif /* HDLINK_CRYPTPROVIDER_HPP_ */
