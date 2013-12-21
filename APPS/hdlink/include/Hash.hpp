#ifndef HDLINK_HASH_HPP_
#define HDLINK_HASH_HPP_

#include <libbase/std.hpp>
#include <libbase/string.hpp>
#include <liblog/logger.hpp>

#include <CryptProvider.hpp>
#include <CryptHasher.hpp>

struct Hash {
	Hash();

	void operator()(const wchar_t * path, uint64_t blockSize = static_cast<uint64_t>(-1)) const;

//	PBYTE hash(const PBYTE hash, size_t sz) const
//	{
//		Memory::copy((PVOID)m_hash, (PCVOID)hash, std::min(size(), sz));
//		return (PBYTE)&m_hash;
//	}
//
//	PBYTE hash() const
//	{
//		return (PBYTE)&m_hash;
//	}

	size_t size() const
	{
		return HASH_SIZE;
	}

	bool is_available() const
	{
		return m_available;
	}

	bool operator ==(const Hash & other) const;

	bool operator !=(const Hash & other) const
	{
		return !this->operator ==(other);
	}

private:
	static const size_t HASH_SIZE = 64;
	BYTE mutable m_hash[HASH_SIZE];
	bool mutable m_available;
};

#endif /* HDLINK_HASH_HPP_ */
