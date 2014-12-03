#include <excis/auth.hpp>

#include <excis/exception.hpp>

#include <basis/sys/sstr.hpp>
#include <basis/simstd/string>

#include <wincred.h>
#include <ntsecapi.h>

namespace auth {

	//typedef enum _CRED_PROTECTION_TYPE {
	//	CredUnprotected         = 0,
	//	CredUserProtection      = 1,
	//	CredTrustedProtection   = 2
	//} CRED_PROTECTION_TYPE, *PCRED_PROTECTION_TYPE;
	//
	//extern "C" {
	//	BOOL WINAPI CredProtectW(BOOL fAsSelf, const wchar_t* pszCredentials, DWORD cchCredentials, PWSTR pszProtectedCredentials, DWORD *pcchMaxChars, CRED_PROTECTION_TYPE *ProtectionType);
	//	BOOL WINAPI CredUnprotectW(BOOL fAsSelf, const wchar_t* pszProtectedCredentials, DWORD cchCredentials, PWSTR pszCredentials, DWORD *pcchMaxChars);
	//}

	Credential::~Credential()
	{
		if (m_delete) {
//			::SecureZeroMemory(m_cred->CredentialBlob, m_cred->CredentialBlobSize);
			::CredFree(reinterpret_cast<PCREDENTIALW>(m_cred));
		}
	}

	Credential::Credential(native_handle_type handle):
		m_cred(reinterpret_cast<native_handle_type>(handle)),
		m_delete(false)
	{
	}

	Credential::Credential(const wchar_t* name):
		m_delete(true)
	{
		CheckApi(::CredReadW(name, CRED_TYPE_GENERIC, 0, reinterpret_cast<CREDENTIALW**>(&m_cred)));
	}

	Credential::Credential(Credential && right):
		m_cred(nullptr),
		m_delete(false)
	{
		swap(right);
	}

	Credential & Credential::operator = (Credential && right)
	{
		if (this != &right)
			Credential(simstd::move(right)).swap(*this);
		return *this;
	}

	void Credential::swap(Credential & right)
	{
		using simstd::swap;
		swap(m_cred, right.m_cred);
		swap(m_delete, right.m_delete);
	}

	ustring Credential::marshal() const
	{
		return ustring(reinterpret_cast<CREDENTIALW*>(m_cred)->Comment);
	}

	ustring Credential::comment() const
	{
		return ustring(reinterpret_cast<CREDENTIALW*>(m_cred)->Comment);
	}

	ustring Credential::name() const
	{
		return ustring(reinterpret_cast<CREDENTIALW*>(m_cred)->TargetName);
	}

	ustring Credential::alias() const
	{
		return ustring(reinterpret_cast<CREDENTIALW*>(m_cred)->TargetAlias);
	}

	ustring Credential::user() const
	{
		return ustring(reinterpret_cast<CREDENTIALW*>(m_cred)->UserName);
	}

	ustring Credential::pass() const
	{
		return ustring();
	}

	size_t Credential::pass_size() const
	{
		return reinterpret_cast<CREDENTIALW*>(m_cred)->CredentialBlobSize;
	}

	size_t Credential::flags() const
	{
		return reinterpret_cast<CREDENTIALW*>(m_cred)->Flags;
	}

	size_t Credential::type() const
	{
		return reinterpret_cast<CREDENTIALW*>(m_cred)->Type;
	}

	size_t Credential::persist() const
	{
		return reinterpret_cast<CREDENTIALW*>(m_cred)->Persist;
	}

	void Credential::add(const wchar_t* name, const wchar_t* pass, const wchar_t* target)
	{
		CREDENTIALW cred;
		memory::zero(cred);
		cred.Flags |= target ? 0 : CRED_FLAGS_USERNAME_TARGET;
		cred.Type = CRED_TYPE_GENERIC;
		cred.Persist = CRED_PERSIST_LOCAL_MACHINE;
		cred.TargetName = const_cast<wchar_t*>(target);
		cred.UserName = const_cast<wchar_t*>(name);
		cred.CredentialBlob = reinterpret_cast<PBYTE>(const_cast<wchar_t*>(pass));
		cred.CredentialBlobSize = sizeof(*pass) * (cstr::length(pass) + 1);
		CheckApi(::CredWriteW(&cred, 0));
	}

	void Credential::del(const wchar_t* name)
	{
		CheckApi(::CredDeleteW(name, CRED_TYPE_GENERIC, 0));
	}

	Credentials::~Credentials()
	{
		::CredFree(m_creds);
	}

	Credentials::Credentials()
	{
		const DWORD CRED_ENUMERATE_ALL_CREDENTIALS = 0x1;

		DWORD size = 0;
		CheckApi(::CredEnumerateW(nullptr, CRED_ENUMERATE_ALL_CREDENTIALS, &size, reinterpret_cast<CREDENTIALW***>(&m_creds)));
		m_size = size;
//		CredentialARGET_INFORMATION TargetInfo = {(PWSTR)L"PC"};
//		CheckApi(::CredReadDomainCredentials(&TargetInfo, 0, &m_size, &m_creds));
	}

	bool Credentials::empty() const
	{
		return m_size == 0;
	}

	size_t Credentials::size() const
	{
		return m_size;
	}

	Credential Credentials::at(size_t ind) const
	{
		CheckApiThrowError(ind < m_size, ERROR_INVALID_INDEX);
		return Credential(reinterpret_cast<Credential::native_handle_type>(reinterpret_cast<CREDENTIALW**>(m_creds)[ind]));
	}

	void parse_username(const wchar_t* fullname, ustring& dom, ustring& name)
	{
		wchar_t d[MAX_PATH];
		wchar_t n[MAX_PATH];
		CheckApiError(::CredUIParseUserNameW(fullname, d, lengthof(d), n, lengthof(n)));
		dom = d;
		name = n;
	}

	void PassProtect(const wchar_t* pass, PWSTR prot, DWORD size)
	{
		CRED_PROTECTION_TYPE type;
		CheckApi(CredProtectW(true, (PWSTR )pass, cstr::length(pass) + 1, prot, &size, &type));
	}

	void PassUnProtect(const wchar_t* prot, DWORD ps, PWSTR pass, DWORD size)
	{
		CheckApi(::CredUnprotectW(true, (PWSTR )prot, ps, pass, &size));
	}

}
