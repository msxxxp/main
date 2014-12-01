#ifndef EXCIS_CRYPT_PROVIDER_HPP_
#define EXCIS_CRYPT_PROVIDER_HPP_

#include <excis/crypt.hpp>

#include <wincrypt.h>

namespace crypt {

	class Provider: private pattern::Uncopyable {
	public:
		~Provider();

		// type = (PROV_RSA_FULL, PROV_RSA_AES)
		Provider(DWORD type = PROV_RSA_FULL);

		// flags = CRYPT_MACHINE_KEYSET
		Provider(const wchar_t* name = nullptr, DWORD flags = CRYPT_VERIFYCONTEXT, const wchar_t* prov = nullptr, DWORD type = PROV_RSA_FULL);

		bool is_exist_key(DWORD type) const;

//		Key create_key(DWORD type, DWORD flags = CRYPT_EXPORTABLE) const;
//
//		Key get_key(DWORD type, DWORD flags = CRYPT_EXPORTABLE) const;

		operator HCRYPTPROV() const;

	private:
		HCRYPTPROV m_hnd;
	};

	inline Provider::operator HCRYPTPROV() const
	{
		return m_hnd;
	}
}

#endif
