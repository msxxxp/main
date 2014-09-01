#include <crypt.hpp>

#include <basis/sys/logger.hpp>

#include <wincrypt.h>

namespace crypt {

	struct Provider_impl: public Provider_i {
		~Provider_impl();

		Provider_impl();

		bool is_valid() const;

		Provider_i::native_handle_t get_native_handle() const override;

	private:
		HCRYPTPROV m_handle;
	};

	Provider_impl::~Provider_impl()
	{
		LogTraceObjBegin();
		::CryptReleaseContext(m_handle, 0);
		LogTraceObjEnd();
	}

	Provider_impl::Provider_impl() :
		m_handle()
	{
		LogTraceObjBegin();
		::CryptAcquireContextW(&m_handle, nullptr, nullptr, PROV_RSA_AES, 0) || ::CryptAcquireContextW(&m_handle, nullptr, nullptr, PROV_RSA_AES, CRYPT_NEWKEYSET);
		LogTraceObjEnd();
	}

	bool Provider_impl::is_valid() const
	{
		return m_handle != HCRYPTPROV();
	}

	Provider_i::native_handle_t Provider_impl::get_native_handle() const
	{
		return reinterpret_cast<Provider_i::native_handle_t>(m_handle);
	}

	Provider provider()
	{
		return Provider(new Provider_impl);
	}
}
