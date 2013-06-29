#ifndef SARALIB_STL_ALGORITHM_SORT_HPP_
#define SARALIB_STL_ALGORITHM_SORT_HPP_

#include "impl_types.hpp"
#include "impl_algorithm_base.hpp"
#include "impl_algorithm_heap.hpp"
#include "impl_functional.hpp"
#include "impl_iterator.hpp"
#include "impl_utility.hpp"

namespace sarastd {
	template<typename ForwardIt, typename T>
	ForwardIt lower_bound(ForwardIt first, ForwardIt last, const T& value)
	{
		ForwardIt it;
		typename sarastd::iterator_traits<ForwardIt>::difference_type count, step;
		count = sarastd::distance(first, last);

		while (count > 0) {
			it = first;
			step = count / 2;
			sarastd::advance(it, step);
			if (*it < value) {
				first = ++it;
				count -= step + 1;
			} else
				count = step;
		}
		return first;
	}

	template<typename ForwardIt, typename T, typename Compare>
	ForwardIt lower_bound(ForwardIt first, ForwardIt last, const T& value, Compare comp)
	{
		ForwardIt it;
		typename sarastd::iterator_traits<ForwardIt>::difference_type count, step;
		count = sarastd::distance(first, last);

		while (count > 0) {
			it = first;
			step = count / 2;
			sarastd::advance(it, step);
			if (comp(*it, value)) {
				first = ++it;
				count -= step + 1;
			} else
				count = step;
		}
		return first;
	}

	template<typename ForwardIt, typename T>
	ForwardIt upper_bound(ForwardIt first, ForwardIt last, const T& value)
	{
		ForwardIt it;
		typename sarastd::iterator_traits<ForwardIt>::distance_type count, step;
		count = sarastd::distance(first, last);

		while (count > 0) {
			it = first;
			step = count / 2;
			sarastd::advance(it, step);
			if (!(value < *it)) {
				first = ++it;
				count -= step + 1;
			} else
				count = step;
		}
		return first;
	}

	template<typename ForwardIt, typename T, typename Compare>
	ForwardIt upper_bound(ForwardIt first, ForwardIt last, const T& value, Compare comp)
	{
		ForwardIt it;
		typename sarastd::iterator_traits<ForwardIt>::distance_type count, step;
		count = sarastd::distance(first, last);

		while (count > 0) {
			it = first;
			step = count / 2;
			sarastd::advance(it, step);
			if (!comp(value, *it)) {
				first = ++it;
				count -= step + 1;
			} else
				count = step;
		}
		return first;
	}

	template<typename ForwardIt, typename T>
	bool binary_search(ForwardIt first, ForwardIt last, const T& value)
	{
		first = sarastd::lower_bound(first, last, value);
		return (!(first == last) && !(value < *first));
	}

	template<typename ForwardIt, typename T, typename Compare>
	bool binary_search(ForwardIt first, ForwardIt last, const T& value, Compare comp)
	{
		first = sarastd::lower_bound(first, last, value, comp);
		return (!(first == last) && !comp(value, *first));
	}

	template<typename ForwardIt, typename T>
	sarastd::pair<ForwardIt, ForwardIt> equal_range(ForwardIt first, ForwardIt last, const T& value)
	{
		return sarastd::make_pair(sarastd::lower_bound(first, last, value), sarastd::upper_bound(first, last, value));
	}

	template<typename ForwardIt, typename T, typename Compare>
	sarastd::pair<ForwardIt, ForwardIt> equal_range(ForwardIt first, ForwardIt last, const T& value, Compare comp)
	{
		return sarastd::make_pair(sarastd::lower_bound(first, last, value, comp), sarastd::upper_bound(first, last, value, comp));
	}

	template<typename InputIt1, typename InputIt2, typename OutputIt, typename Compare>
	OutputIt merge(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2, OutputIt d_first, Compare comp)
	{
		for (; first1 != last1; ++d_first) {
			if (first2 == last2) {
				return sarastd::copy(first1, last1, d_first);
			}
			if (comp(*first2, *first1)) {
				*d_first = *first2;
				++first2;
			} else {
				*d_first = *first1;
				++first1;
			}
		}
		return sarastd::copy(first2, last2, d_first);
	}

	//	template<typename InputIterator1, typename InputIterator2, typename OutputIterator, typename Compare>
	//	OutputIterator merge(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result, Compare compare)
	//	{
	//		while ((first1 != last1) && (first2 != last2)) {
	//			if (compare(*first2, *first1)) {
	//				*result = *first2;
	//				++first2;
	//			} else {
	//				*result = *first1;
	//				++first1;
	//			}
	//			++result;
	//		}
	//		return sarastd::copy(first2, last2, sarastd::copy(first1, last1, result));
	//	}

	template<typename InputIterator1, typename InputIterator2, typename OutputIterator>
	OutputIterator merge(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result)
	{
		typedef sarastd::less<typename sarastd::iterator_traits<InputIterator1>::value_type> less;
		return sarastd::merge<InputIterator1, InputIterator2, OutputIterator, less>(first1, last1, first2, last2, result, less());
	}

	namespace pvt {
		const sarastd::ptrdiff_t ISORT_MAX = 32; // maximum size for insertion sort

		template<typename T, typename Compare>
		const T& median(const T& a, const T& b, const T& c, Compare compare)
		{
			if (compare(a, b)) {
				if (compare(b, c))
					return b;
				else if (compare(a, c))
					return c;
				else
					return a;
			} else if (compare(a, c))
				return a;
			else if (compare(b, c))
				return c;
			return b;
		}

		template<typename RandomIterator, typename T, typename Compare>
		RandomIterator get_partition(RandomIterator first, RandomIterator last, const T& pivotValue, Compare compare)
		{
			const T pivotCopy(pivotValue);

			for (;; ++first) {
				while (compare(*first, pivotCopy)) {
					++first;
				}
				--last;
				while (compare(pivotCopy, *last)) {
					--last;
				}

				if (first >= last)
					return first;
				sarastd::iter_swap(first, last);
			}
		}

		template<typename BidirectionalIterator, typename Compare>
		void insertion_sort(BidirectionalIterator first, BidirectionalIterator last, Compare compare)
		{
			typedef typename sarastd::iterator_traits<BidirectionalIterator>::value_type value_type;

			if (first != last) {
				BidirectionalIterator iCurrent, iNext, iSorted = first;

				for (++iSorted; iSorted != last; ++iSorted) {
					const value_type temp(*iSorted);

					iNext = iCurrent = iSorted;

					for (--iCurrent; (iNext != first) && compare(temp, *iCurrent); --iNext, --iCurrent) {
						*iNext = *iCurrent;
					}

					*iNext = temp;
				}
			}
		}

		template<typename RandomIterator, typename Compare>
		void insertion_sort_simple(RandomIterator first, RandomIterator last, Compare compare)
		{
			for (RandomIterator current = first; current != last; ++current) {
				typedef typename sarastd::iterator_traits<RandomIterator>::value_type value_type;

				RandomIterator end(current), prev(current);
				const value_type value(*current);

				for (--prev; compare(value, *prev); --end, --prev) { // We skip checking for (prev >= first) because quick_sort (our caller) makes this unnecessary.
					*end = *prev;
				}

				*end = value;
			}
		}

		template<typename RandomIterator, typename Compare>
		void shell_sort(RandomIterator first, RandomIterator last, Compare compare)
		{
			typedef typename sarastd::iterator_traits<RandomIterator>::difference_type difference_type;

			// We use the Knuth 'h' sequence below, as it is easy to calculate at runtime.
			// However, possibly we are better off using a different sequence based on a table.
			// One such sequence which averages slightly better than Knuth is:
			//    1, 5, 19, 41, 109, 209, 505, 929, 2161, 3905, 8929, 16001, 36289,
			//    64769, 146305, 260609, 587521, 1045505, 2354689, 4188161, 9427969, 16764929

			if (first != last) {
				RandomIterator iCurrent, iBack, iSorted, iInsertFirst;
				difference_type nSize = last - first;
				difference_type nSpace = 1; // nSpace is the 'h' value of the ShellSort algorithm.

				while (nSpace < nSize)
					nSpace = (nSpace * 3) + 1; // This is the Knuth 'h' sequence: 1, 4, 13, 40, 121, 364, 1093, 3280, 9841, 29524, 88573, 265720, 797161, 2391484, 7174453, 21523360, 64570081, 193710244,

				for (nSpace = (nSpace - 1) / 3; nSpace >= 1; nSpace = (nSpace - 1) / 3)  // Integer division is less than ideal.
				    {
					for (difference_type i = 0; i < nSpace; i++) {
						iInsertFirst = first + i;

						for (iSorted = iInsertFirst + nSpace; iSorted < last; iSorted += nSpace) {
							iBack = iCurrent = iSorted;

							for (iBack -= nSpace; (iCurrent != iInsertFirst) && compare(*iCurrent, *iBack); iCurrent = iBack, iBack -= nSpace) {
								sarastd::iter_swap(iCurrent, iBack);
							}
						}
					}
				}
			}
		}

		template<typename RandomIterator, typename Compare>
		void heap_sort(RandomIterator first, RandomIterator last, Compare compare)
		{
			// We simply call our heap algorithms to do the work for us.
			sarastd::make_heap<RandomIterator, Compare>(first, last, compare);
			sarastd::sort_heap<RandomIterator, Compare>(first, last, compare);
		}

		template<typename RandomIterator, typename T, typename Compare>
		void merge_sort_buffer(RandomIterator first, RandomIterator last, T* pBuffer, Compare compare)
		{
			typedef typename sarastd::iterator_traits<RandomIterator>::difference_type difference_type;
			const difference_type nCount = last - first;

			if (nCount > 1) {
				const difference_type nMid = nCount / 2;
				RandomIterator half = first + nMid;

				if (nMid > 1) {
					const difference_type nQ1(nMid / 2);
					RandomIterator part(first + nQ1);

					sarastd::pvt::merge_sort_buffer<RandomIterator, T, Compare>(first, part, pBuffer, compare);
					sarastd::pvt::merge_sort_buffer<RandomIterator, T, Compare>(part, half, pBuffer + nQ1, compare);
					sarastd::merge<RandomIterator, RandomIterator, T*, Compare>(first, part, part, half, pBuffer, compare);
				} else
					*pBuffer = *first;

				if ((nCount - nMid) > 1) {
					const difference_type nQ3((nMid + nCount) / 2);
					RandomIterator part(first + nQ3);

					sarastd::pvt::merge_sort_buffer<RandomIterator, T, Compare>(half, part, pBuffer + nMid, compare);
					sarastd::pvt::merge_sort_buffer<RandomIterator, T, Compare>(part, last, pBuffer + nQ3, compare);
					sarastd::merge<RandomIterator, RandomIterator, T*, Compare>(half, part, part, last, pBuffer + nMid, compare);
				} else
					*(pBuffer + nMid) = *half;

				sarastd::merge<T*, T*, RandomIterator, Compare>(pBuffer, pBuffer + nMid, pBuffer + nMid, pBuffer + nCount, first, compare);
			}
		}

		template<typename RandomIterator, typename Compare>
		void merge_sort(RandomIterator first, RandomIterator last, Compare compare)
		{
			typedef typename sarastd::iterator_traits<RandomIterator>::difference_type difference_type;
			typedef typename sarastd::iterator_traits<RandomIterator>::value_type value_type;

			const difference_type nCount = last - first;

			if (nCount > 1) {
				// We need to allocate an array of nCount value_type objects as a temporary buffer.
				sarastd::pair<value_type*, sarastd::ptrdiff_t> pBuffer = sarastd::get_temporary_buffer(nCount);
				sarastd::uninitialized_fill(pBuffer.first, pBuffer.first + nCount, value_type());

				sarastd::pvt::merge_sort_buffer<RandomIterator, value_type, Compare>(first, last, pBuffer, compare);

				sarastd::pvt::_destroy<value_type*, value_type>(pBuffer.first, pBuffer.first + nCount);
				sarastd::return_temporary_buffer(pBuffer.first);
			}
		}

		//	template<typename RandomIterator, typename _Size, typename _Compare>
		//	void _introselect(RandomIterator __first, RandomIterator __nth, RandomIterator __last, _Size __depth_limit, _Compare __comp)
		//	{
		//		typedef typename iterator_traits<RandomIterator>::value_type value_type;
		//
		//		while (__last - __first > 3) {
		//			if (__depth_limit == 0) {
		//				std::__heap_select(__first, __nth + 1, __last, __comp);
		//				// Place the nth largest element in its final position.
		//				sarastd::iter_swap(__first, __nth);
		//				return;
		//			}
		//			--__depth_limit;
		//			RandomIterator __cut = std::__unguarded_partition_pivot(__first, __last, __comp);
		//			if (__cut <= __nth)
		//				__first = __cut;
		//			else
		//				__last = __cut;
		//		}
		//		std::__insertion_sort(__first, __last, __comp);
		//	}
		//
		//	template<typename RandomIterator, typename _Compare>
		//	void nth_element(RandomIterator __first, RandomIterator __nth, RandomIterator __last, _Compare __comp)
		//	{
		//		typedef typename iterator_traits<RandomIterator>::value_type value_type;
		//
		//		if (__first == __last || __nth == __last)
		//			return;
		//
		//		std::__introselect(__first, __nth, __last, std::__lg(__last - __first) * 2, __comp);
		//	}

	}

	///=============================================================================================
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

	///=============================================================================================
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

	template<typename ForwardIt>
	ForwardIt is_sorted_until(ForwardIt first, ForwardIt last)
	{
		typedef sarastd::less<typename sarastd::iterator_traits<ForwardIt>::value_type> less;
		return sarastd::is_sorted_until<ForwardIt, less>(first, last, less());
	}

	template<typename ForwardIt, typename Compare>
	bool is_sorted(ForwardIt first, ForwardIt last, Compare comp)
	{
		return sarastd::is_sorted_until(first, last, comp) == last;
	}

	template<typename ForwardIt>
	bool is_sorted(ForwardIt first, ForwardIt last)
	{
		typedef sarastd::less<typename sarastd::iterator_traits<ForwardIt>::value_type> less;
		return sarastd::is_sorted_until<ForwardIt, less>(first, last, less()) == last;
	}

	template<typename RandomIterator, typename Compare>
	void partial_sort(RandomIterator first, RandomIterator middle, RandomIterator last, Compare compare)
	{
		typedef typename sarastd::iterator_traits<RandomIterator>::difference_type difference_type;
		typedef typename sarastd::iterator_traits<RandomIterator>::value_type value_type;

		sarastd::make_heap<RandomIterator, Compare>(first, middle, compare);

		for (RandomIterator i = middle; i < last; ++i) {
			if (compare(*i, *first)) {
				const value_type temp(*i);
				*i = *first;
				sarastd::pvt::adjust_heap<RandomIterator, difference_type, value_type, Compare>(first, difference_type(0), difference_type(middle - first), difference_type(0), temp, compare);
			}
		}

		sarastd::sort_heap<RandomIterator, Compare>(first, middle, compare);
	}

	template<typename RandomIterator>
	void partial_sort(RandomIterator first, RandomIterator middle, RandomIterator last)
	{
		typedef sarastd::less<typename sarastd::iterator_traits<RandomIterator>::value_type> less;
		sarastd::partial_sort<RandomIterator, less>(first, middle, last, less());
	}

	namespace pvt {
		template<typename RandomIterator, typename Size, typename Compare>
		void quick_sort_impl(RandomIterator first, RandomIterator last, Size kRecursionCount, Compare compare)
		{
			typedef typename iterator_traits<RandomIterator>::value_type value_type;

			while (((last - first) > ISORT_MAX) && (kRecursionCount > 0)) {
				const RandomIterator position(
				    sarastd::pvt::get_partition<RandomIterator, value_type, Compare>(first, last,
				                                                                     sarastd::pvt::median<value_type, Compare>(*first, *(first + (last - first) / 2), *(last - 1), compare), compare));

				sarastd::pvt::quick_sort_impl<RandomIterator, Size, Compare>(position, last, --kRecursionCount, compare);
				last = position;
			}

			if (kRecursionCount == 0)
				sarastd::partial_sort<RandomIterator, Compare>(first, last, last, compare);
		}

		template<typename RandomIterator, typename Compare>
		void quick_sort(RandomIterator first, RandomIterator last, Compare compare)
		{
			typedef typename sarastd::iterator_traits<RandomIterator>::difference_type difference_type;

			if (first != last) {
				sarastd::pvt::quick_sort_impl<RandomIterator, difference_type, Compare>(first, last, 2 * sarastd::Log2(last - first), compare);

				if ((last - first) > (difference_type)ISORT_MAX) {
					sarastd::pvt::insertion_sort<RandomIterator, Compare>(first, first + ISORT_MAX, compare);
					sarastd::pvt::insertion_sort_simple<RandomIterator, Compare>(first + ISORT_MAX, last, compare);
				} else
					sarastd::pvt::insertion_sort<RandomIterator, Compare>(first, last, compare);
			}
		}
	}

	template<typename RandomIterator, typename Compare>
	void sort(RandomIterator first, RandomIterator last, Compare predicate)
	{
		sarastd::pvt::quick_sort(first, last, predicate);
	}

	template<typename RandomIterator>
	void sort(RandomIterator first, RandomIterator last)
	{
		typedef sarastd::less<typename sarastd::iterator_traits<RandomIterator>::value_type> less;
		sarastd::sort<RandomIterator, less>(first, last, less());
	}

	template<typename RandomIterator, typename Compare>
	void stable_sort(RandomIterator first, RandomIterator last, Compare compare)
	{
		sarastd::pvt::merge_sort<RandomIterator, Compare>(first, last, compare);
	}

	template<typename RandomIterator>
	void stable_sort(RandomIterator first, RandomIterator last)
	{
		typedef sarastd::less<typename sarastd::iterator_traits<RandomIterator>::value_type> less;
		sarastd::pvt::merge_sort<RandomIterator, less>(first, last, less());
	}

	template<typename RandomIterator, typename Compare>
	void nth_element(RandomIterator first, RandomIterator nth, RandomIterator last, Compare compare)
	{
		typedef typename iterator_traits<RandomIterator>::value_type value_type;

		while ((last - first) > 5) {
			const value_type midValue(sarastd::pvt::median<value_type, Compare>(*first, *(first + (last - first) / 2), *(last - 1), compare));
			const RandomIterator midPos(sarastd::pvt::get_partition<RandomIterator, value_type, Compare>(first, last, midValue, compare));

			if (midPos <= nth)
				first = midPos;
			else
				last = midPos;
		}
		sarastd::pvt::insertion_sort<RandomIterator, Compare>(first, last, compare);
	}

	template<typename RandomIterator>
	void nth_element(RandomIterator first, RandomIterator nth, RandomIterator last)
	{
		typedef sarastd::less<typename sarastd::iterator_traits<RandomIterator>::value_type> less;
		sarastd::nth_element<RandomIterator, less>(first, nth, last, less());
	}

}

#endif
