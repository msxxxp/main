#include <basis/sys/crypt.hpp>
#include <basis/sys/logger.hpp>

#include <wincrypt.h>

namespace crypt {

	struct HashImpl: public Hash {
		~HashImpl();

		HashImpl(const ProviderHolder& provider);

		bool is_valid() const;

		bool process(const void* buf, size_t size) override;

		size_t get_size() const override;

		void get_hash(void* buf, size_t size) const override;

	private:
		HCRYPTHASH m_handle;
	};

	HashImpl::~HashImpl()
	{
		LogTraceObjBegin();
		if (m_handle)
			::CryptDestroyHash(m_handle);
		LogTraceObjEnd();
	}

	HashImpl::HashImpl(const ProviderHolder& provider)
	{
		LogTraceObjBegin();
		::CryptCreateHash(reinterpret_cast<HCRYPTPROV>(provider->get_native_handle()), CALG_SHA1, 0, 0, &m_handle);
		LogTraceObjEnd();
	}

	bool HashImpl::is_valid() const
	{
		return m_handle != HCRYPTHASH();
	}

	bool HashImpl::process(const void* buf, size_t size)
	{
		return ::CryptHashData(m_handle, (PBYTE)buf, size, 0);
	}

	size_t HashImpl::get_size() const
	{
		DWORD ret = 0;
		DWORD ret_size = sizeof(ret);
		::CryptGetHashParam(m_handle, HP_HASHSIZE, (PBYTE)&ret, &ret_size, 0);
		return ret;
	}

	void HashImpl::get_hash(void* buf, size_t size) const
	{
		DWORD l_size = static_cast<DWORD>(size);
		::CryptGetHashParam(m_handle, HP_HASHVAL, (PBYTE)buf, &l_size, 0);
	}

	HashHolder hash(const ProviderHolder& provider)
	{
		simstd::unique_ptr<HashImpl> tmp(new HashImpl(provider));
		return tmp->is_valid() ? HashHolder(simstd::move(tmp)) : HashHolder();
	}

}
