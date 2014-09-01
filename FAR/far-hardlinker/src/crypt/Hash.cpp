#include <crypt.hpp>

#include <basis/sys/logger.hpp>

#include <wincrypt.h>

namespace crypt {

	struct Hash_impl: public Hash_i {
		~Hash_impl();

		Hash_impl(const Provider & provider);

		bool is_valid() const;

		bool process(const void * buf, size_t size) override;

		size_t get_size() const override;

		void get_hash(void * buf, size_t size) const override;

	private:
		HCRYPTHASH m_handle;
	};

	Hash_impl::~Hash_impl()
	{
		LogTraceObjBegin();
		if (m_handle)
			::CryptDestroyHash(m_handle);
		LogTraceObjEnd();
	}

	Hash_impl::Hash_impl(const Provider & provider)
	{
		LogTraceObjBegin();
		::CryptCreateHash(reinterpret_cast<HCRYPTPROV>(provider->get_native_handle()), CALG_SHA1, 0, 0, &m_handle);
		LogTraceObjEnd();
	}

	bool Hash_impl::is_valid() const
	{
		return m_handle != HCRYPTHASH();
	}

	bool Hash_impl::process(const void * buf, size_t size)
	{
		return ::CryptHashData(m_handle, (PBYTE)buf, size, 0);
	}

	size_t Hash_impl::get_size() const
	{
		DWORD ret = 0;
		DWORD ret_size = sizeof(ret);
		::CryptGetHashParam(m_handle, HP_HASHSIZE, (PBYTE)&ret, &ret_size, 0);
		return ret;
	}

	void Hash_impl::get_hash(void * buf, size_t size) const
	{
		DWORD l_size = static_cast<DWORD>(size);
		::CryptGetHashParam(m_handle, HP_HASHVAL, (PBYTE)buf, &l_size, 0);
	}

	Hash hash(const Provider & provider)
	{
		return Hash(new Hash_impl(provider));
	}

}
