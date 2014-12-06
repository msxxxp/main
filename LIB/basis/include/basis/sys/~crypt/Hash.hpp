#ifndef BASIS_SYS_CRYPT_HASH_HPP_
#define BASIS_SYS_CRYPT_HASH_HPP_

namespace crypt {

	class Hash {
		struct native_impl_type;

	public:
		typedef native_impl_type* native_handle_type;

		virtual ~Hash() = default;

		virtual bool process(const void* buf, size_t size) = 0;

		virtual size_t get_size() const = 0;

		virtual void get_hash(void* buf, size_t size) const = 0;
	};

	typedef simstd::unique_ptr<Hash> HashHolder;

	HashHolder hash(const ProviderHolder& provider);

}

#endif
