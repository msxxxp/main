#ifndef EXCIS_CRYPT_HASH_HPP_
#define EXCIS_CRYPT_HASH_HPP_

#include <excis/crypt.hpp>

#include <wincrypt.h>

namespace crypt {

	struct Hash {
		~Hash();

		// alg = (CALG_MD5, CALG_SHA1, CALG_SHA_512)
		Hash(HCRYPTPROV prov, ALG_ID alg);

		Hash(HCRYPTHASH hash);

//		Hash(const fsys::File::Facade & wf, uint64_t size = (uint64_t)-1);
//
//		Hash(const fsys::File::Map & file);

		Hash(const Hash & other);

		Hash & operator=(const Hash & other);

		void process(const void * buf, size_t size);

		size_t get_size() const;

		ALG_ID get_algorithm() const;

		void get_hash(void * buf, size_t size) const;

		operator HCRYPTHASH() const;

		void swap(Hash & rhs);

	private:
		HCRYPTHASH m_handle;
	};

	inline Hash::operator HCRYPTHASH() const
	{
		return m_handle;
	}

}

#endif
