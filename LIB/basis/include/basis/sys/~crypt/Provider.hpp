#ifndef BASIS_SYS_CRYPT_PROVIDER_HPP_
#define BASIS_SYS_CRYPT_PROVIDER_HPP_

namespace crypt {

	class Provider {
		struct native_impl_type;

	public:
		typedef native_impl_type* native_handle_type;

		virtual ~Provider() = default;

		virtual native_handle_type get_native_handle() const = 0;
	};

	typedef simstd::unique_ptr<Provider> ProviderHolder;

	ProviderHolder provider();

}

#endif
