#ifndef HDLINK_CRYPT_HPP_
#define HDLINK_CRYPT_HPP_

#include <basis/ext/pattern.hpp>

namespace crypt {

	struct Provider: private pattern::Uncopyable {
		typedef uintptr_t native_handle_type;

		~Provider();

		Provider();

		bool is_valid() const;

		operator native_handle_type() const;

	private:
		native_handle_type m_handle;
	};

	inline Provider::operator Provider::native_handle_type() const
	{
		return m_handle;
	}

	struct Hash: private pattern::Uncopyable {
		typedef uintptr_t native_handle_type;

		~Hash();

		Hash(const Provider & provider);

		bool is_valid() const;

		bool process(const void * buf, size_t size);

		size_t get_size() const;

		void get_hash(void * buf, size_t size) const;

		operator native_handle_type() const;

	private:
		native_handle_type m_handle;
	};

	inline Hash::operator Hash::native_handle_type() const
	{
		return m_handle;
	}

}

#endif
