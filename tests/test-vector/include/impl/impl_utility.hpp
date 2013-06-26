#ifndef SARALIB_STL_UTILITY_HPP_
#define SARALIB_STL_UTILITY_HPP_

#include "impl_types.hpp"
#include "impl_algorithm_base.hpp"

namespace sarastd {
	template<class T1, class T2>
	struct pair {
		typedef T1 first_type;
		typedef T2 second_type;

		first_type first;
		second_type second;

		pair();
		pair(const T1& x, const T2& y);
		pair(const pair& p);

		template<class U1, class U2>
		pair(const pair<U1, U2>& p);

		template<class U1, class U2>
		pair& operator =(const pair<U1, U2>& other);
		pair& operator =(const pair& other);

		void swap(pair& other);
	};

	template<class T1, class T2>
	pair<T1, T2>::pair() :
		first(), second()
	{
	}

	template<class T1, class T2>
	pair<T1, T2>::pair(const T1& x, const T2& y) :
		first(x), second(y)
	{
	}

	template<class T1, class T2>
	pair<T1, T2>::pair(const pair& p) :
		first(p.first), second(p.second)
	{
	}

	template<class T1, class T2>
	template<class U1, class U2>
	pair<T1, T2>::pair(const pair<U1, U2>& p) :
		first(p.first), second(p.second)
	{
	}

	template<class T1, class T2>
	pair<T1, T2>& pair<T1, T2>::operator=(const pair& other)
	{
		pair(other).swap(*this);
		return *this;
	}

	template<class T1, class T2>
	template<class U1, class U2>
	pair<T1, T2>& pair<T1, T2>::operator=(const pair<U1, U2>& other)
	{
		pair(other).swap(*this);
		return *this;
	}

	template<class T1, class T2>
	void pair<T1, T2>::swap(pair& other)
	{
		using sarastd::swap;
		swap(first, other.first);
		swap(second, other.second);
	}

	///=============================================================================================
	template<class T1, class T2>
	sarastd::pair<T1, T2> make_pair(T1 t, T2 u)
	{
		return sarastd::pair<T1, T2>(t, u);
	}

	template<class T1, class T2>
	bool operator ==(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
	{
		return lhs.first == rhs.first && lhs.second == rhs.second;
	}

	template<class T1, class T2>
	bool operator !=(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
	{
		return !(lhs == rhs);
	}

	template<class T1, class T2>
	bool operator <(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
	{
		if (lhs.first < rhs.first)
			return true;
		else if (rhs.first < lhs.first)
			return false;
		else if (lhs.second < rhs.second)
			return true;
		else
			return false;
	}

	template<class T1, class T2>
	bool operator <=(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
	{
		return !(rhs < lhs);
	}

	template<class T1, class T2>
	bool operator >(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
	{
		return rhs < lhs;
	}

	template<class T1, class T2>
	bool operator >=(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
	{
		return !(lhs < rhs);
	}

	template<class T1, class T2>
	void swap(pair<T1, T2>& lhs, pair<T1, T2>& rhs)
	{
		lhs.swap(rhs);
	}

}

#endif
