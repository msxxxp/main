#include <libext/exception.hpp>
#include <CryptHasher.hpp>

CryptHasher::~CryptHasher()
{
	if (m_hnd) {
		::CryptDestroyHash(m_hnd);
	}
}

CryptHasher::CryptHasher(const CryptProvider & prov, ALG_ID alg) :
	m_hnd()
{
	CheckApi(::CryptCreateHash(prov, alg, 0, 0, &m_hnd));
}

void CryptHasher::process(const PBYTE buf, size_t size)
{
	CheckApi(::CryptHashData(m_hnd, buf, size, 0));
}

void CryptHasher::process(const wchar_t * /*path*/, uint64_t /*size*/)
{
//	FileMap file(path, size);
//	while (file.Next())
//		Hash((const PBYTE)file.data(), file.size());
}

size_t CryptHasher::get_size() const
{
	DWORD ret = 0;
	CheckApi(::CryptGetHashParam(m_hnd, HP_HASHVAL, nullptr, &ret, 0));
	return ret;
}

ALG_ID CryptHasher::get_algorithm() const
{
	DWORD ret = 0;
	CheckApi(::CryptGetHashParam(m_hnd, HP_ALGID, nullptr, &ret, 0));
	return ret;
}

void CryptHasher::get_raw_hash(PBYTE buf, DWORD size) const
{
	CheckApi(::CryptGetHashParam(m_hnd, HP_HASHVAL, buf, &size, 0));
}
