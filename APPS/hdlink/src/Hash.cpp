#include <libext/exception.hpp>
#include <Hash.hpp>
#include <Statistics.hpp>

Hash::Hash() :
	m_available(false)
{
	Memory::zero(m_hash, HASH_SIZE);
}

void Hash::operator()(const wchar_t * path, uint64_t blockSize) const
{
	static CryptProvider provider(nullptr, PROV_RSA_AES);

	if (provider.is_valid()) {
		CryptHasher hSHA(provider, CALG_SHA1);
		if (hSHA.process(path, fsize)) {
			++statistics().hashesCalculated;
			m_available = hSHA.get_hash(m_hash, HASH_SIZE);
			return m_available;
		}
		LogFatal(L"%s\n", path);
	}
	LogFatal(L"Unable to count hash: %s\n", ErrAsStr().c_str());

	return false;
}

bool Hash::operator ==(const Hash & other) const
{
	++statistics().hashCompares;
	return Memory::compare(m_hash, other.m_hash, size());
}

