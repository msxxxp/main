#ifndef BASIS_EXTRA_PATTERN_INITED_HPP_
#define BASIS_EXTRA_PATTERN_INITED_HPP_

namespace pattern {

	template<typename Type, unsigned long long DEFAULT = 0ULL>
	struct inited
	{
		typedef Type value_type;

		inited() :
			m_value(static_cast<value_type>(DEFAULT))
		{
		}

		inited(const value_type& other) :
			m_value(other)
		{
		}

		inited& operator =(const value_type& other)
		{
			m_value = other;
			return *this;
		}

		operator value_type& ()
		{
			return m_value;
		}

		operator const value_type& () const
		{
			return m_value;
		}

		value_type& operator-> ()
		{
			return m_value;
		}

		const value_type& operator-> () const
		{
			return m_value;
		}

	private:
		value_type m_value;
	};

}

#endif
