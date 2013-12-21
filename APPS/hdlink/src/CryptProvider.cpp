#include <libext/exception.hpp>
#include <liblog/logger.hpp>
#include <CryptProvider.hpp>

CryptProvider::~CryptProvider()
{
	if (m_hnd) {
		::CryptReleaseContext(m_hnd, 0);
	}
}

CryptProvider::CryptProvider(PCWSTR prov, DWORD type) :
	m_hnd(nullptr)
{
	::CryptAcquireContextW(&m_hnd, L"MY", prov, type, 0) || ::CryptAcquireContextW(&m_hnd, L"MY", prov, type, CRYPT_NEWKEYSET);
	LogErrorIf(!m_hnd, L"Unable to initialize Crypto Provider\n");
	CheckHandle(m_hnd);
}
