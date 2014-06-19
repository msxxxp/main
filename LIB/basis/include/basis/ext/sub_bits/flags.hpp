#ifndef LIBBASE_BIT_HPP_
#define LIBBASE_BIT_HPP_

#include <stdint.h>

namespace Base {

	template<typename EnumType>
	class BitFlags {
		typedef BitFlags this_type;

	public:
		typedef uint32_t value_type;

		BitFlags():
			m_data()
		{
		}

		BitFlags(unsigned long val):
			m_data(val)
		{
		}

		BitFlags(const this_type & right):
			m_data(right.m_data)
		{
		}

		BitFlags & operator =(const this_type & right)
		{
			this_type(right).swap(*this);
			return *this;
		}

		void swap(this_type & right)
		{
			uint32_t tmp = m_data;
			m_data = right.m_data;
			right.m_data = tmp;
		}

		bool operator ==(const this_type & right)
		{
			return m_data == right.m_data;
		}

		bool operator !=(const this_type & right)
		{
			return m_data != right.m_data;
		}

		bool operator [](EnumType pos) const
		{
			uint32_t bit = uint32_t(1) << (int)pos;
			return m_data & bit;
		}

		bool test(EnumType pos) const
		{
			uint32_t bit = uint32_t(1) << (int)pos;
			return m_data & bit;
		}

		bool all() const
		{
			uint32_t bits = ~uint32_t(0);
			return (m_data & bits) == m_data;
		}

		bool any() const
		{
			return m_data;
		}

		bool none() const
		{
			uint32_t bits = ~uint32_t(0);
			return (m_data & bits) == uint32_t(0);
		}

		size_t count() const
		{
			return 0;
		}

		size_t size() const
		{
			return sizeof(uint32_t) * 8;
		}

		this_type operator ~() const
		{
			return this_type(~m_data);
		}

		this_type operator <<(size_t pos) const
		{
			return this_type(m_data << pos);
		}

		this_type operator >>(size_t pos) const
		{
			return this_type(m_data >> pos);
		}

		this_type & operator &=(const this_type & other)
		{
			m_data &= other.m_data;
			return *this;
		}

		this_type & operator |=(const this_type& other)
		{
			m_data |= other.m_data;
			return *this;
		}

		this_type & operator ^=(const this_type& other)
		{
			m_data ^= other.m_data;
			return *this;
		}

		this_type & operator <<=(size_t pos)
		{
			m_data <<= pos;
			return *this;
		}

		this_type & operator >>=(size_t pos)
		{
			m_data >>= pos;
			return *this;
		}

		this_type & set()
		{
			uint32_t bits = ~uint32_t(0);
			m_data |= bits;
			return *this;
		}

		this_type & set(EnumType pos, bool value = true)
		{
			m_data &= ~(uint32_t(1) << (int)pos);
			m_data |= uint32_t(value) << (int)pos;
			return *this;
		}

		this_type & reset()
		{
			m_data = uint32_t(0);
			return *this;
		}

		this_type & reset(EnumType pos)
		{
			m_data &= ~(uint32_t(1) << (int)pos);
			return *this;
		}

		this_type & flip()
		{
			m_data = ~m_data;
			return *this;
		}

		this_type & flip(EnumType pos)
		{
			return set(pos, !test(pos));
		}

		unsigned long to_ulong() const
		{
			return m_data;
		}

	private:
		uint32_t m_data;
	};

	template<typename EnumType>
	BitFlags<EnumType> operator &(const BitFlags<EnumType> & left, const BitFlags<EnumType> & right)
	{
		return BitFlags<EnumType>(left) &= right;
	}

	template<typename EnumType>
	BitFlags<EnumType> operator |(const BitFlags<EnumType> & left, const BitFlags<EnumType> & right)
	{
		return BitFlags<EnumType>(left) |= right;
	}

	template<typename EnumType>
	BitFlags<EnumType> operator ^(const BitFlags<EnumType> & left, const BitFlags<EnumType> & right)
	{
		return BitFlags<EnumType>(left) ^= right;
	}
}

#endif
