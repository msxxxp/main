#include <crypt.hpp>

#include <basis/sys/logger.hpp>

#include <wincrypt.h>

namespace crypt {

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
		return m_handle != native_handle_type();
	}

}
