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

	template<typename InputIt1, typename InputIt2>
	bool equal(InputIt1 first1, InputIt1 last1, InputIt2 first2)
	{
		for (; first1 != last1; ++first1, ++first2)
			if (!(*first1 == *first2))
				return false;
		return true;
	}

}

#endif
