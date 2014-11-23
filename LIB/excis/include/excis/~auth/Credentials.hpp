#ifndef EXCIS_AUTH_CREDENTIALS_HPP_
#define EXCIS_AUTH_CREDENTIALS_HPP_

#include <excis/auth.hpp>

namespace auth {

	struct Credentials {
		struct native_imp_type;

	public:
		typedef native_imp_type * native_handle_type;

		~Credentials();

		Credentials();

		bool empty() const;

		size_t size() const;

		Credential at(size_t ind) const;

	private:
		native_handle_type m_creds;
		size_t             m_size;
	};

	void parse_username(const wchar_t * fullname, ustring & dom, ustring & name);

}

#endif
