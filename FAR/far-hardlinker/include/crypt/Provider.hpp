#ifndef _CRYPT_PROVIDER_HPP_
#define _CRYPT_PROVIDER_HPP_

#include <crypt.hpp>

namespace crypt {

	class Provider_i {
		struct native_impl_t;

	public:
		typedef native_impl_t * native_handle_t;

		virtual ~Provider_i() = default;

		virtual native_handle_t get_native_handle() const = 0;
	};

	typedef simstd::unique_ptr<Provider_i> Provider;

	Provider provider();

}

#endif
