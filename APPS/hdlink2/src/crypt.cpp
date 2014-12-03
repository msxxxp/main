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

	Hash::~Hash()
	{
		LogTraceObjBegin();
		if (m_handle)
			::CryptDestroyHash(m_handle);
		LogTraceObjEnd();
	}

	Hash::Hash(const Provider & provider)
	{
		LogTraceObjBegin();
		::CryptCreateHash(provider, CALG_SHA1, 0, 0, &m_handle);
		LogTraceObjEnd();
	}

	bool Hash::is_valid() const
	{
		return m_handle != native_handle_type();
	}

	bool Hash::process(const void * buf, size_t size)
	{
		return ::CryptHashData(m_handle, (PBYTE)buf, size, 0);
	}

	size_t Hash::get_size() const
	{
		DWORD ret = 0;
		DWORD ret_size = sizeof(ret);
		::CryptGetHashParam(m_handle, HP_HASHSIZE, (PBYTE)&ret, &ret_size, 0);
		return ret;
	}

	void Hash::get_hash(void * buf, size_t size) const
	{
		DWORD l_size = static_cast<DWORD>(size);
		::CryptGetHashParam(m_handle, HP_HASHVAL, (PBYTE)buf, &l_size, 0);
	}

}
