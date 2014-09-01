#include <crypt.hpp>

#include <basis/sys/logger.hpp>

#include <wincrypt.h>

namespace crypt {
	struct Provider: public Provider_i {
		~Provider();

		Provider();

		bool is_valid() const;

		Provider_i::native_handle_t get_native_handle() const;

	private:
		HCRYPTPROV m_handle;
	};

	Provider::~Provider()
	{
		LogTraceObjBegin();
		::CryptReleaseContext(m_handle, 0);
		LogTraceObjEnd();
	}

	Provider::Provider() :
		m_handle()
	{
		LogTraceObjBegin();
		::CryptAcquireContextW(&m_handle, nullptr, nullptr, PROV_RSA_AES, 0) || ::CryptAcquireContextW(&m_handle, nullptr, nullptr, PROV_RSA_AES, CRYPT_NEWKEYSET);
		LogTraceObjEnd();
	}

	bool Provider::is_valid() const
	{
		return m_handle != HCRYPTPROV();
	}

	Provider_i::native_handle_t Provider::get_native_handle() const
	{
		return reinterpret_cast<HCRYPTPROV>(m_handle);
	}

	Provider provider()
	{
	}
}
