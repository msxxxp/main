#ifndef BASIS_CONFIGURE_SIMSTD_ALGORITHM_HPP_
#define BASIS_CONFIGURE_SIMSTD_ALGORITHM_HPP_

namespace simstd {

	template<typename Type>
	void swap(Type & a, Type & b)
	{
		Type tmp(simstd::move(a));
		a = simstd::move(b);
		b = simstd::move(tmp);
	}

	template<typename Type, size_t Num>
	void swap(Type (&a)[Num], Type (&b)[Num])
	{
		for (size_t n = 0; n < Num; ++n)
			swap(a[n], b[n]);
	}

#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

	template<typename T>
	const T& min(const T& a, const T& b)
	{
		return (b < a) ? b : a;
	}

	template<typename T, typename Comparator>
	const T& min(const T& a, const T& b, Comparator comp)
	{
		return comp(b, a) ? b : a;
	}

	template<typename T>
	const T& max(const T& a, const T& b)
	{
		return (b < a) ? a : b;
	}

	template<typename T, typename Comparator>
	const T& max(const T& a, const T& b, Comparator comp)
	{
		return comp(b, a) ? a : b;
	}

	template<typename T>
	bool between(const T& a, const T& val, const T& b)
	{
		return (a <= val) && (val <= b);
	}

	template<typename T>
	bool between_strict(const T& a, const T& val, const T& b)
	{
		return (a < val) && (val < b);
	}

}

#endif
