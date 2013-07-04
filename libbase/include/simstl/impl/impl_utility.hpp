#ifndef SARALIB_STL_UTILITY_HPP_
#define SARALIB_STL_UTILITY_HPP_

#include "impl_types.hpp"
#include "impl_algorithm_base.hpp"

namespace sarastd {
	namespace pvt {
		namespace ops {
			template<typename T1, typename T2>
			bool operator !=(const T1& x, const T2& y)
			{
				return !(x == y);
			}

			template<typename T1, typename T2>
			bool operator >(const T1& x, const T2& y)
			{
				return (y < x);
			}

			template<typename T1, typename T2>
			bool operator <=(const T1& x, const T2& y)
			{
				return !(y < x);
			}

			template<typename T1, typename T2>
			bool operator >=(const T1& x, const T2& y)
			{
				return !(x < y);
			}
		}
	}

	template<typename T1, typename T2>
	struct pair {
		typedef T1 first_type;
		typedef T2 second_type;

		first_type first;
		second_type second;

		pair();
		pair(const T1& x, const T2& y);

		template<typename U1, typename U2>
		pair(const pair<U1, U2>& p);

		template<typename U1, typename U2>
		pair& operator =(const pair<U1, U2>& other);

		void swap(pair& other);
	};

	template<typename T1, typename T2>
	pair<T1, T2>::pair() :
		first(), second()
	{
	}

	template<typename T1, typename T2>
	pair<T1, T2>::pair(const T1& x, const T2& y) :
		first(x), second(y)
	{
	}

	template<typename T1, typename T2>
	template<typename U1, typename U2>
	pair<T1, T2>::pair(const pair<U1, U2>& p) :
		first(p.first), second(p.second)
	{
	}

	template<typename T1, typename T2>
	template<typename U1, typename U2>
	pair<T1, T2>& pair<T1, T2>::operator=(const pair<U1, U2>& other)
	{
		pair(other).swap(*this);
		return *this;
	}

	template<typename T1, typename T2>
	void pair<T1, T2>::swap(pair& other)
	{
#if _GNUC_ < 3
		sarastd::swap(first, other.first);
		sarastd::swap(second, other.second);
#else
		using sarastd::swap;
		swap(first, other.first);
		swap(second, other.second);
#endif
	}

	///=============================================================================================
	template<typename T1, typename T2>
	sarastd::pair<T1, T2> make_pair(T1 t, T2 u)
	{
		return sarastd::pair<T1, T2>(t, u);
	}

	template<typename T1, typename T2>
	bool operator ==(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
	{
		return lhs.first == rhs.first && lhs.second == rhs.second;
	}

	template<typename T1, typename T2>
	bool operator !=(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
	{
		return pvt::ops::operator !=(lhs, rhs);
	}

	template<typename T1, typename T2>
	bool operator <(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
	{
        return ((lhs.first < rhs.first) || (!(rhs.first < lhs.first) && (lhs.second < rhs.second)));
	}

	template<typename T1, typename T2>
	bool operator <=(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
	{
		return pvt::ops::operator <=(lhs, rhs);
	}

	template<typename T1, typename T2>
	bool operator >(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
	{
		return pvt::ops::operator >(lhs, rhs);
	}

	template<typename T1, typename T2>
	bool operator >=(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
	{
		return pvt::ops::operator >=(lhs, rhs);
	}

	template<typename T1, typename T2>
	void swap(pair<T1, T2>& lhs, pair<T1, T2>& rhs)
	{
		lhs.swap(rhs);
	}

}

#endif
