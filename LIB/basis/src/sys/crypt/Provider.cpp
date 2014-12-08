#include <basis/sys/crypt.hpp>
#include <basis/sys/logger.hpp>
#include <basis/sys/totext.hpp>

#include <wincrypt.h>

namespace crypt {

	struct ProviderImpl: public Provider {
		~ProviderImpl();

		ProviderImpl();

		bool is_valid() const;

		Provider::native_handle_type get_native_handle() const override;

	private:
		HCRYPTPROV m_handle;
	};

	ProviderImpl::~ProviderImpl()
	{
		LogTraceObjBegin();
		bool ok = ::CryptReleaseContext(m_handle, 0);
		LogErrorIf(!ok, L"-> %s\n", totext::api_error().c_str());
		LogTraceObjEnd();
	}

	ProviderImpl::ProviderImpl() :
		m_handle()
	{
		LogTraceObjBegin();
		bool ok = ::CryptAcquireContextW(&m_handle, nullptr, nullptr, PROV_RSA_AES, 0) || ::CryptAcquireContextW(&m_handle, nullptr, nullptr, PROV_RSA_AES, CRYPT_NEWKEYSET);
		LogErrorIf(!ok, L"-> %s\n", totext::api_error().c_str());
		LogTraceObjEnd();
	}

	bool ProviderImpl::is_valid() const
	{
		return m_handle != HCRYPTPROV();
	}

	Provider::native_handle_type ProviderImpl::get_native_handle() const
	{
		return reinterpret_cast<Provider::native_handle_type>(m_handle);
	}

	ProviderHolder provider()
	{
		simstd::unique_ptr<ProviderImpl> tmp(new ProviderImpl);
		return tmp->is_valid() ? ProviderHolder(simstd::move(tmp)) : ProviderHolder();
	}
}
