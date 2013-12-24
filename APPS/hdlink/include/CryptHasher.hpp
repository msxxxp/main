#ifndef HDLINK_CRYPTHASH_HPP_
#define HDLINK_CRYPTHASH_HPP_

#include <CryptProvider.hpp>

struct CryptHasher {
	~CryptHasher();

	// alg = (CALG_MD5, CALG_SHA1, CALG_SHA_512)
	CryptHasher(const CryptProvider & prov, ALG_ID alg);

	operator HCRYPTHASH() const
	{
		return m_hnd;
	}

	void process(const PBYTE buf, size_t size);

	void process(const wchar_t * path, uint64_t size = static_cast<uint64_t>(-1));

	size_t get_size() const;

	ALG_ID get_algorithm() const;

	void get_raw_hash(PBYTE buf, DWORD size) const;

private:
	HCRYPTHASH m_hnd;
};

#endif /* HDLINK_CRYPTHASH_HPP_ */
