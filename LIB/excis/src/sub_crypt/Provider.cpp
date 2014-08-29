#include <excis/crypt.hpp>
#include <excis/exception.hpp>

#include <basis/sys/logger.hpp>

namespace crypt {

	Provider::~Provider()
	{
		LogTraceObjBegin();
		::CryptReleaseContext(m_hnd, 0);
		LogTraceObjEnd();
	}

	Provider::Provider(DWORD type) :
		Provider(nullptr, CRYPT_VERIFYCONTEXT, nullptr, type)
	{
	}

	Provider::Provider(PCWSTR name, DWORD flags, PCWSTR prov, DWORD type) :
		m_hnd()
	{
		LogTraceObjBegin();
		if (!::CryptAcquireContextW(&m_hnd, name, prov, type, flags))
			CheckApi(::CryptAcquireContextW(&m_hnd, name, prov, type, flags | CRYPT_NEWKEYSET));
		LogTraceObjEnd();
	}

	bool Provider::is_exist_key(DWORD type) const
	{
		HCRYPTKEY key = (HCRYPTKEY)nullptr;
		if (::CryptGetUserKey(m_hnd, type, &key)) {
			return ::CryptDestroyKey(key);
		}
		DWORD err = ::GetLastError();
		if (err != (DWORD)NTE_NO_KEY) {
			CheckApiError(err);
		}
		return false;
	}

//	Key Provider::create_key(DWORD type, DWORD flags) const
//	{
//		HCRYPTKEY key = (HCRYPTKEY)nullptr;
//		CheckApi(::CryptGenKey(m_hnd, type, flags, &key));
//		return Key(key);
//	}
//
//	Key Provider::get_key(DWORD type, DWORD flags) const
//	{
//		HCRYPTKEY key = (HCRYPTKEY)nullptr;
//		if (!::CryptGetUserKey(m_hnd, type, &key))
//			CheckApi(::CryptGenKey(m_hnd, type, flags, &key));
//		return Key(key);
//	}

}
