#ifndef SARALIB_STL_NUMERIC_HPP_
#define SARALIB_STL_NUMERIC_HPP_

#include "impl_types.hpp"
#include "impl_functional.hpp"
#include "impl_utility.hpp"

namespace sarastd {
	template<typename ForwardIterator, typename T>
	void iota(ForwardIterator first, ForwardIterator last, T value)
	{
		while (first != last) {
			*first++ = value;
			++value;
		}
	}

	template<typename InputIt, typename T>
	T accumulate(InputIt first, InputIt last, T init)
	{
		for (; first != last; ++first) {
			init = init + *first;
		}
		return init;
	}

	template<typename InputIt, typename T, typename BinaryOperation>
	T accumulate(InputIt first, InputIt last, T init, BinaryOperation op)
	{
		for (; first != last; ++first) {
			init = op(init, *first);
		}
		return init;
	}

	template<typename InputIt1, typename InputIt2, typename T>
	T inner_product(InputIt1 first1, InputIt1 last1, InputIt2 first2, T value)
	{
		while (first1 != last1) {
			value = value + *first1 * *first2;
			++first1;
			++first2;
		}
		return value;
	}

	template<typename InputIt1, typename InputIt2, typename T, typename BinaryOperation1, typename BinaryOperation2>
	T inner_product(InputIt1 first1, InputIt1 last1, InputIt2 first2, T value, BinaryOperation1 op1, BinaryOperation2 op2)
	{
		while (first1 != last1) {
			value = op1(value, op2(*first1, *first2));
			++first1;
			++first2;
		}
		return value;
	}

	template<typename InputIt, typename OutputIt>
	OutputIt adjacent_difference(InputIt first, InputIt last, OutputIt d_first)
	{
		if (first == last)
			return d_first;

		typedef typename sarastd::iterator_traits<InputIt>::value_type value_t;
		value_t acc = *first;
		*d_first = acc;
		while (++first != last) {
			value_t val = *first;
			*++d_first = val - acc;
			acc = val;
		}
		return ++d_first;
	}

	template<typename InputIt, typename OutputIt, typename BinaryOperation>
	OutputIt adjacent_difference(InputIt first, InputIt last, OutputIt d_first, BinaryOperation op)
	{
		if (first == last)
			return d_first;

		typedef typename sarastd::iterator_traits<InputIt>::value_type value_t;
		value_t acc = *first;
		*d_first = acc;
		while (++first != last) {
			value_t val = *first;
			*++d_first = op(val, acc);
			acc = val;
		}
		return ++d_first;
	}

	template<typename InputIt, typename OutputIt, typename BinaryOperator>
	OutputIt partial_sum(InputIt first, InputIt last, OutputIt d_first, BinaryOperator op)
	{
		if (first == last)
			return d_first;

		typename sarastd::iterator_traits<InputIt>::value_type sum = *first;
		*d_first = sum;

		while (++first != last) {
			sum = op(sum, *first);
			*++d_first = sum;
		}
		return ++d_first;
	}

	template<typename InputIt, typename OutputIt>
	OutputIt partial_sum(InputIt first, InputIt last, OutputIt d_first)
	{
		return sarastd::partial_sum(first, last, d_first, sarastd::plus<typename sarastd::iterator_traits<InputIt>::value_type>());
	}

}

#endif
