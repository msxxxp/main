#include <libext/exception.hpp>
#include <Hash.hpp>
#include <global.hpp>

Hash::Hash() :
	m_available(false)
{
	Memory::zero(m_hash, HASH_SIZE);
}

void Hash::operator()(const wchar_t * path, uint64_t blockSize) const
{
	CryptHasher hasher(*Global::cryptProvider, CALG_SHA1);
	hasher.process(path, blockSize);
	hasher.get_raw_hash(m_hash, HASH_SIZE);
	++statistics().hashesCalculated;
	m_available = true;
}

bool Hash::operator ==(const Hash & other) const
{
	++statistics().hashCompares;
	return Memory::compare(m_hash, other.m_hash, size());
}

