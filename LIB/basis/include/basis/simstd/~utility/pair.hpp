#ifndef BASIS_PAIR_HPP_
#define BASIS_PAIR_HPP_

#include <basis/simstd/~algorithm/base.hpp>
#include <basis/simstd/~utility/rel_ops.hpp>

namespace simstd {

	template<typename T1, typename T2>
	struct pair
	{
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
		first(),
		second()
	{
	}

	template<typename T1, typename T2>
	pair<T1, T2>::pair(const T1& x, const T2& y) :
		first(x),
		second(y)
	{
	}

	template<typename T1, typename T2>
	template<typename U1, typename U2>
	pair<T1, T2>::pair(const pair<U1, U2>& p) :
		first(p.first),
		second(p.second)
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
#if defined(__GNUC__) && (__GNUC__ < 3)
		simstd::swap(first, other.first);
		simstd::swap(second, other.second);
#else
		using simstd::swap;
		swap(first, other.first);
		swap(second, other.second);
#endif
	}

	///=============================================================================================
	template<typename T1, typename T2>
	simstd::pair<T1, T2> make_pair(T1 t, T2 u)
	{
		return simstd::pair<T1, T2>(t, u);
	}

	template<typename T1, typename T2>
	bool operator ==(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
	{
		return lhs.first == rhs.first && lhs.second == rhs.second;
	}

	template<typename T1, typename T2>
	bool operator <(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
	{
		return ((lhs.first < rhs.first) || (!(rhs.first < lhs.first) && (lhs.second < rhs.second)));
	}

	template<typename T1, typename T2>
	bool operator !=(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
	{
		return rel_ops::operator !=(lhs, rhs);
	}

	template<typename T1, typename T2>
	bool operator <=(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
	{
		return rel_ops::operator <=(lhs, rhs);
	}

	template<typename T1, typename T2>
	bool operator >(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
	{
		return rel_ops::operator >(lhs, rhs);
	}

	template<typename T1, typename T2>
	bool operator >=(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
	{
		return rel_ops::operator >=(lhs, rhs);
	}

	template<typename T1, typename T2>
	void swap(pair<T1, T2>& lhs, pair<T1, T2>& rhs)
	{
		lhs.swap(rhs);
	}

}

#endif
