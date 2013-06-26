#ifndef SARALIB_STL_ALGORITHM_SORT_HPP_
#define SARALIB_STL_ALGORITHM_SORT_HPP_

#include "impl_types.hpp"
#include "impl_algorithm_base.hpp"
#include "impl_iterator.hpp"
#include "impl_utility.hpp"

namespace sarastd {
	template<typename InputIt, typename UnaryPredicate>
	bool is_partitioned(InputIt first, InputIt last, UnaryPredicate p)
	{
		for (; first != last; ++first)
			if (!p(*first))
				break;
		for (; first != last; ++first)
			if (p(*first))
				return false;
		return true;
	}

	template<typename BidirIt, typename UnaryPredicate>
	BidirIt partition(BidirIt first, BidirIt last, UnaryPredicate p)
	{
		while (1) {
			while ((first != last) && p(*first)) {
				++first;
			}
			if (first == last--)
				break;
			while ((first != last) && !p(*last)) {
				--last;
			}
			if (first == last)
				break;
			sarastd::iter_swap(first, last);
			++first;
		}
		return first;
	}

	template<typename ForwardIt, typename Compare>
	ForwardIt is_sorted_until(ForwardIt first, ForwardIt last, Compare comp)
	{
		if (first != last) {
			ForwardIt next = first;
			while (++next != last) {
				if (comp(*next, *first))
					return next;
				first = next;
			}
		}
		return last;
	}

	template<typename ForwardIt, typename Compare>
	bool is_sorted(ForwardIt first, ForwardIt last, Compare comp)
	{
		return sarastd::is_sorted_until(first, last, comp) == last;
	}

	const sarastd::ptrdiff_t ISORT_MAX = 32; // maximum size for insertion sort

//	template<typename RanIt, typename _Size>
//	void _introselect(RanIt __first, RanIt __nth, RanIt __last, _Size __depth_limit)
//	{
//		typedef typename iterator_traits<RanIt>::value_type value_type;
//
//		while (__last - __first > 3) {
//			if (__depth_limit == 0) {
//				std::__heap_select(__first, __nth + 1, __last);
//
//				// Place the nth largest element in its final position.
//				sarastd::iter_swap(__first, __nth);
//				return;
//			}
//			--__depth_limit;
//			RanIt __cut = std::__unguarded_partition_pivot(__first, __last);
//			if (__cut <= __nth)
//				__first = __cut;
//			else
//				__last = __cut;
//		}
//		std::__insertion_sort(__first, __last);
//	}
//
//	template<typename RanIt, typename _Size, typename _Compare>
//	void _introselect(RanIt __first, RanIt __nth, RanIt __last, _Size __depth_limit, _Compare __comp)
//	{
//		typedef typename iterator_traits<RanIt>::value_type value_type;
//
//		while (__last - __first > 3) {
//			if (__depth_limit == 0) {
//				std::__heap_select(__first, __nth + 1, __last, __comp);
//				// Place the nth largest element in its final position.
//				sarastd::iter_swap(__first, __nth);
//				return;
//			}
//			--__depth_limit;
//			RanIt __cut = std::__unguarded_partition_pivot(__first, __last, __comp);
//			if (__cut <= __nth)
//				__first = __cut;
//			else
//				__last = __cut;
//		}
//		std::__insertion_sort(__first, __last, __comp);
//	}
//
//	template<typename RanIt>
//	void nth_element(RanIt __first, RanIt __nth, RanIt __last)
//	{
//		typedef typename iterator_traits<RanIt>::value_type value_type;
//
//		if (__first == __last || __nth == __last)
//			return;
//
//		sarastd::_introselect(__first, __nth, __last, std::_lg(__last - __first) * 2);
//	}
//
//	template<typename RanIt, typename _Compare>
//	void nth_element(RanIt __first, RanIt __nth, RanIt __last, _Compare __comp)
//	{
//		typedef typename iterator_traits<RanIt>::value_type value_type;
//
//		if (__first == __last || __nth == __last)
//			return;
//
//		std::__introselect(__first, __nth, __last, std::__lg(__last - __first) * 2, __comp);
//	}

	template<typename BiIt>
	void _insertion_sort(BiIt first, BiIt last)
	{
		if (first == last)
			return;

		BiIt i = first;
		for (++i; i != last; ++i) {
			BiIt j = i, j1 = i;
			for (; j != first && *j < *--j1; --j)
				sarastd::iter_swap(j, j1);
		}
	}

	template<typename BiIt, typename Compare>
	void _insertion_sort(BiIt first, BiIt last, Compare predicate)
	{
		if (first == last)
			return;

		BiIt i = first;
		for (++i; i != last; ++i) {
			BiIt j = i, j1 = i;
			for (; j != first && predicate(*j, *--j1); --j)
				sarastd::iter_swap(j, j1);
		}
	}

	template<typename RanIt>
	sarastd::pair<RanIt, RanIt> _partition(RanIt first, RanIt last)
	{
		typedef typename sarastd::iterator_traits<RanIt>::value_type value_type;
		value_type middle = *(first + ((last - first) / 2));

		RanIt i = first;
		for (RanIt k = i; k != last; ++k)
			if (*k < middle) {
				sarastd::iter_swap(i, k);
				++i;
			}

		RanIt j = i;
		for (RanIt k = j; k != last; ++k)
			if (!(middle < *k)) {
				sarastd::iter_swap(j, k);
				++j;
			}
		return sarastd::make_pair(i, j);
	}

	template<typename RanIt, typename Compare>
	pair<RanIt, RanIt> _partition(RanIt first, RanIt last, Compare predicate)
	{
		typedef typename sarastd::iterator_traits<RanIt>::value_type value_type;
		value_type middle = *(first + ((last - first) / 2));

		RanIt i = first;
		for (RanIt k = i; k != last; ++k)
			if (predicate(*k, middle)) {
				sarastd::iter_swap(i, k);
				++i;
			}

		RanIt j = i;
		for (RanIt k = j; k != last; ++k)
			if (!predicate(middle, *k)) {
				sarastd::iter_swap(j, k);
				++j;
			}

		return make_pair(i, j);
	}

	template<typename RanIt>
	void _sort(RanIt first, RanIt last)
	{
		while (last - first > ISORT_MAX) {
			pair<RanIt, RanIt> middle = _partition(first, last);
			if (middle.first - first >= last - middle.second) {
				_sort(middle.second, last);
				last = middle.first;
			} else {
				_sort(first, middle.first);
				first = middle.second;
			}
		}
		_insertion_sort(first, last);
	}

	template<typename RanIt, typename Compare>
	void _sort(RanIt first, RanIt last, Compare predicate)
	{
		while (last - first > ISORT_MAX) {
			pair<RanIt, RanIt> middle = _partition(first, last, predicate);
			if (middle.first - first >= last - middle.second) {
				_sort(middle.second, last, predicate);
				last = middle.first;
			} else {
				_sort(first, middle.first, predicate);
				first = middle.second;
			}
		}
		_insertion_sort(first, last, predicate);
	}

	template<typename RanIt>
	RanIt _quick_sort_split(RanIt first, RanIt last)
	{
		RanIt pivot = --last;
		sarastd::iter_swap(pivot, first + (last - first) / 2);
		while (true) {
			while (*first < *pivot)
				++first;
			if (first == last)
				break;
			--last;
			while ((first != last) && !(*last < *pivot)) {
				--last;
			}
			if (first == last)
				break;
			sarastd::iter_swap(first, last);
			++first;
		}
		sarastd::iter_swap(pivot, first);
		return first;
	}

	template<typename RanIt>
	void _quick_sort(RanIt first, RanIt last)
	{
		RanIt middle = _quick_sort_split(first, last);
		for (RanIt it = first; it != last; ++it)
		{
			printf(" %d", it->value());
		}
		printf("\n");
		if (first < middle) {
			_quick_sort(first, middle);
		}
		if (middle < last) {
			_quick_sort(middle, last);
		}
	}

	template<typename RanIt>
	void sort(RanIt first, RanIt last)
	{
		for (RanIt it = first; it != last; ++it)
		{
			printf(" %d", it->value());
		}
		printf("\n");
		_quick_sort(first, last);
//		_sort(first, last);
	}

	template<typename RanIt, typename Compare>
	void sort(RanIt first, RanIt last, Compare predicate)
	{
		_sort(first, last, predicate);
	}
}

#endif
