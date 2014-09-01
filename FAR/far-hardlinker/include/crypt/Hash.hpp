#ifndef _CRYPT_HASH_HPP_
#define _CRYPT_HASH_HPP_

#include <crypt.hpp>

namespace crypt {

	class Hash_i {
		struct native_impl_t;

	public:
		typedef native_impl_t * native_handle_t;

		virtual ~Hash_i() = default;

		virtual bool process(const void * buf, size_t size) = 0;

		virtual size_t get_size() const = 0;

		virtual void get_hash(void * buf, size_t size) const = 0;
	};

	typedef simstd::unique_ptr<Hash_i> Hash;

	Hash hash(const Provider & provider);

}

#endif
