#ifndef EXCIS_AUTH_CREDENTIAL_HPP_
#define EXCIS_AUTH_CREDENTIAL_HPP_

#include <excis/auth.hpp>

namespace auth {

	class Credential {
		struct native_imp_type;

	public:
		typedef native_imp_type * native_handle_type;

		~Credential();

		Credential(const wchar_t * name);

		Credential(Credential && right);

		Credential & operator = (Credential && right);

		void swap(Credential & right);

		ustring marshal() const;

		ustring comment() const;

		ustring name() const;

		ustring alias() const;

		ustring user() const;

		ustring pass() const;

		size_t pass_size() const;

		size_t flags() const;

		size_t type() const;

		size_t persist() const;

		static void add(const wchar_t * name, const wchar_t * pass, const wchar_t * target = nullptr);

		static void del(const wchar_t * name);

	private:
		native_handle_type m_cred;
		bool               m_delete;

		Credential(native_handle_type handle);

		friend struct Credentials;
	};

}

#endif
