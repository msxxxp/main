#ifndef LIBSTL_ALGORITHM_SORT_HPP_
#define LIBSTL_ALGORITHM_SORT_HPP_

#include <simstl/types.hpp>
#include <simstl/impl/functional.hpp>
#include <simstl/impl/iterator/base.hpp>
#include <simstl/impl/utility/pair.hpp>
#include <simstl/impl/memory/base.hpp>
#include <simstl/impl/memory/functional.hpp>

namespace simstd {
	template<typename ForwardIt, typename T>
	ForwardIt lower_bound(ForwardIt first, ForwardIt last, const T& value)
	{
		typename simstd::iterator_traits<ForwardIt>::difference_type count = simstd::distance(first, last);

		while (count > 0) {
			typename simstd::iterator_traits<ForwardIt>::difference_type step = count / 2;
			ForwardIt mid = simstd::next(first, step);
			if (*mid < value) {
				first = ++mid;
				count -= step + 1;
			} else {
				count = step;
			}
		}

		return first;
	}

	template<typename ForwardIt, typename T, typename Compare>
	ForwardIt lower_bound(ForwardIt first, ForwardIt last, const T& value, Compare comp)
	{
		typename simstd::iterator_traits<ForwardIt>::difference_type count = simstd::distance(first, last);

		while (count > 0) {
			typename simstd::iterator_traits<ForwardIt>::difference_type step = count / 2;
			ForwardIt mid = simstd::next(first, step);
			if (comp(*mid, value)) {
				first = ++mid;
				count -= step + 1;
			} else {
				count = step;
			}
		}

		return first;
	}

	template<typename ForwardIt, typename T>
	ForwardIt upper_bound(ForwardIt first, ForwardIt last, const T& value)
	{
		typename simstd::iterator_traits<ForwardIt>::difference_type count = simstd::distance(first, last);

		while (count > 0) {
			typename simstd::iterator_traits<ForwardIt>::difference_type step = count / 2;
			ForwardIt mid = simstd::next(first, step);
			if (!(value < *mid)) {
				first = ++mid;
				count -= step + 1;
			} else {
				count = step;
			}
		}

		return first;
	}

	template<typename ForwardIt, typename T, typename Compare>
	ForwardIt upper_bound(ForwardIt first, ForwardIt last, const T& value, Compare comp)
	{
		typename simstd::iterator_traits<ForwardIt>::difference_type count = simstd::distance(first, last);

		while (count > 0) {
			typename simstd::iterator_traits<ForwardIt>::difference_type step = count / 2;
			ForwardIt mid = simstd::next(first, step);
			if (!comp(value, *mid)) {
				first = ++mid;
				count -= step + 1;
			} else {
				count = step;
			}
		}

		return first;
	}

	template<typename ForwardIt, typename T>
	bool binary_search(ForwardIt first, ForwardIt last, const T& value)
	{
		return (value < *first) ? false : simstd::lower_bound(first, last, value) != last;
	}

	template<typename ForwardIt, typename T, typename Compare>
	bool binary_search(ForwardIt first, ForwardIt last, const T& value, Compare comp)
	{
		return comp(value, *first) ? false : simstd::lower_bound(first, last, value, comp) != last;
	}

	template<typename ForwardIt, typename T>
	simstd::pair<ForwardIt, ForwardIt> equal_range(ForwardIt first, ForwardIt last, const T& value)
	{
		return simstd::make_pair(simstd::lower_bound(first, last, value), simstd::upper_bound(first, last, value));
	}

	template<typename ForwardIt, typename T, typename Compare>
	simstd::pair<ForwardIt, ForwardIt> equal_range(ForwardIt first, ForwardIt last, const T& value, Compare comp)
	{
		return simstd::make_pair(simstd::lower_bound(first, last, value, comp), simstd::upper_bound(first, last, value, comp));
	}

	template<typename InputIt1, typename InputIt2, typename OutputIt, typename Compare>
	OutputIt merge(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2, OutputIt d_first, Compare comp)
	{
		for (; first1 != last1; ++d_first) {
			if (first2 == last2) {
				return simstd::copy(first1, last1, d_first);
			}
			if (comp(*first2, *first1)) {
				*d_first = *first2;
				++first2;
			} else {
				*d_first = *first1;
				++first1;
			}
		}
		return simstd::copy(first2, last2, d_first);
	}

	template<typename InputIt1, typename InputIt2, typename OutputIt>
	OutputIt merge(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2, OutputIt result)
	{
		typedef simstd::less<typename simstd::iterator_traits<InputIt1>::value_type> less;
		return simstd::merge<InputIt1, InputIt2, OutputIt, less>(first1, last1, first2, last2, result, less());
	}

	namespace pvt {
		const ptrdiff_t ISORT_MAX = 32; // maximum size for insertion sort

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

		template<typename RandomIt, typename T, typename Compare>
		RandomIt get_partition(RandomIt first, RandomIt last, const T& pivotValue, Compare compare)
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
				simstd::iter_swap(first, last);
			}
		}

		template<typename BidirectionalIterator, typename Compare>
		void insertion_sort(BidirectionalIterator first, BidirectionalIterator last, Compare compare)
		{
			typedef typename simstd::iterator_traits<BidirectionalIterator>::value_type value_type;

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

		template<typename RandomIt, typename Compare>
		void insertion_sort_simple(RandomIt first, RandomIt last, Compare compare)
		{
			for (RandomIt current = first; current != last; ++current) {
				typedef typename simstd::iterator_traits<RandomIt>::value_type value_type;

				RandomIt end(current), prev(current);
				const value_type value(*current);

				for (--prev; compare(value, *prev); --end, --prev) { // We skip checking for (prev >= first) because quick_sort (our caller) makes this unnecessary.
					*end = *prev;
				}

				*end = value;
			}
		}

		template<typename RandomIt, typename Compare>
		void shell_sort(RandomIt first, RandomIt last, Compare compare)
		{
			typedef typename simstd::iterator_traits<RandomIt>::difference_type difference_type;

			// We use the Knuth 'h' sequence below, as it is easy to calculate at runtime.
			// However, possibly we are better off using a different sequence based on a table.
			// One such sequence which averages slightly better than Knuth is:
			//    1, 5, 19, 41, 109, 209, 505, 929, 2161, 3905, 8929, 16001, 36289,
			//    64769, 146305, 260609, 587521, 1045505, 2354689, 4188161, 9427969, 16764929

			if (first != last) {
				RandomIt iCurrent, iBack, iSorted, iInsertFirst;
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
								simstd::iter_swap(iCurrent, iBack);
							}
						}
					}
				}
			}
		}

		template<typename RandomIt, typename Compare>
		void heap_sort(RandomIt first, RandomIt last, Compare compare)
		{
			// We simply call our heap algorithms to do the work for us.
			simstd::make_heap<RandomIt, Compare>(first, last, compare);
			simstd::sort_heap<RandomIt, Compare>(first, last, compare);
		}

		template<typename RandomIt, typename T, typename Compare>
		void merge_sort_buffer(RandomIt first, RandomIt last, T* pBuffer, Compare compare)
		{
			typedef typename simstd::iterator_traits<RandomIt>::difference_type difference_type;
			const difference_type nCount = last - first;

			if (nCount > 1) {
				const difference_type nMid = nCount / 2;
				RandomIt half = first + nMid;

				if (nMid > 1) {
					const difference_type nQ1(nMid / 2);
					RandomIt part(first + nQ1);

					simstd::pvt::merge_sort_buffer<RandomIt, T, Compare>(first, part, pBuffer, compare);
					simstd::pvt::merge_sort_buffer<RandomIt, T, Compare>(part, half, pBuffer + nQ1, compare);
					simstd::merge<RandomIt, RandomIt, T*, Compare>(first, part, part, half, pBuffer, compare);
				} else
					*pBuffer = *first;

				if ((nCount - nMid) > 1) {
					const difference_type nQ3((nMid + nCount) / 2);
					RandomIt part(first + nQ3);

					simstd::pvt::merge_sort_buffer<RandomIt, T, Compare>(half, part, pBuffer + nMid, compare);
					simstd::pvt::merge_sort_buffer<RandomIt, T, Compare>(part, last, pBuffer + nQ3, compare);
					simstd::merge<RandomIt, RandomIt, T*, Compare>(half, part, part, last, pBuffer + nMid, compare);
				} else
					*(pBuffer + nMid) = *half;

				simstd::merge<T*, T*, RandomIt, Compare>(pBuffer, pBuffer + nMid, pBuffer + nMid, pBuffer + nCount, first, compare);
			}
		}

		template<typename RandomIt, typename Compare>
		void merge_sort(RandomIt first, RandomIt last, Compare compare)
		{
			typedef typename simstd::iterator_traits<RandomIt>::difference_type difference_type;
			typedef typename simstd::iterator_traits<RandomIt>::value_type value_type;

			const difference_type nCount = last - first;

			if (nCount > 1) {
				// We need to allocate an array of nCount value_type objects as a temporary buffer.
				simstd::pair<value_type*, ptrdiff_t> pBuffer = simstd::get_temporary_buffer(nCount);
				simstd::uninitialized_fill(pBuffer.first, pBuffer.first + nCount, value_type());

				simstd::pvt::merge_sort_buffer<RandomIt, value_type, Compare>(first, last, pBuffer, compare);

				simstd::pvt::_destroy<value_type*, value_type>(pBuffer.first, pBuffer.first + nCount);
				simstd::return_temporary_buffer(pBuffer.first);
			}
		}

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
			simstd::iter_swap(first, last);
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
		typedef simstd::less<typename simstd::iterator_traits<ForwardIt>::value_type> less;
		return simstd::is_sorted_until<ForwardIt, less>(first, last, less());
	}

	template<typename ForwardIt, typename Compare>
	bool is_sorted(ForwardIt first, ForwardIt last, Compare comp)
	{
		return simstd::is_sorted_until(first, last, comp) == last;
	}

	template<typename ForwardIt>
	bool is_sorted(ForwardIt first, ForwardIt last)
	{
		typedef simstd::less<typename simstd::iterator_traits<ForwardIt>::value_type> less;
		return simstd::is_sorted_until<ForwardIt, less>(first, last, less()) == last;
	}

	template<typename RandomIt, typename Compare>
	void partial_sort(RandomIt first, RandomIt middle, RandomIt last, Compare compare)
	{
		typedef typename simstd::iterator_traits<RandomIt>::difference_type difference_type;
		typedef typename simstd::iterator_traits<RandomIt>::value_type value_type;

		simstd::make_heap<RandomIt, Compare>(first, middle, compare);

		for (RandomIt i = middle; i < last; ++i) {
			if (compare(*i, *first)) {
				const value_type temp(*i);
				*i = *first;
				simstd::pvt::adjust_heap<RandomIt, difference_type, value_type, Compare>(first, difference_type(0), difference_type(middle - first), difference_type(0), temp, compare);
			}
		}

		simstd::sort_heap<RandomIt, Compare>(first, middle, compare);
	}

	template<typename RandomIt>
	void partial_sort(RandomIt first, RandomIt middle, RandomIt last)
	{
		typedef simstd::less<typename simstd::iterator_traits<RandomIt>::value_type> less;
		simstd::partial_sort<RandomIt, less>(first, middle, last, less());
	}

	namespace pvt {
		template<typename RandomIt, typename Size, typename Compare>
		void quick_sort_impl(RandomIt first, RandomIt last, Size kRecursionCount, Compare compare)
		{
			typedef typename iterator_traits<RandomIt>::value_type value_type;

			while (((last - first) > ISORT_MAX) && (kRecursionCount > 0)) {
				const RandomIt position(
				    simstd::pvt::get_partition<RandomIt, value_type, Compare>(first, last, simstd::pvt::median<value_type, Compare>(*first, *(first + (last - first) / 2), *(last - 1), compare),
				                                                               compare));

				simstd::pvt::quick_sort_impl<RandomIt, Size, Compare>(position, last, --kRecursionCount, compare);
				last = position;
			}

			if (kRecursionCount == 0)
				simstd::partial_sort<RandomIt, Compare>(first, last, last, compare);
		}

		template<typename RandomIt, typename Compare>
		void quick_sort(RandomIt first, RandomIt last, Compare compare)
		{
			typedef typename simstd::iterator_traits<RandomIt>::difference_type difference_type;

			if (first != last) {
				simstd::pvt::quick_sort_impl<RandomIt, difference_type, Compare>(first, last, 2 * simstd::pvt::Log2(last - first), compare);

				if ((last - first) > (difference_type)ISORT_MAX) {
					simstd::pvt::insertion_sort<RandomIt, Compare>(first, first + ISORT_MAX, compare);
					simstd::pvt::insertion_sort_simple<RandomIt, Compare>(first + ISORT_MAX, last, compare);
				} else
					simstd::pvt::insertion_sort<RandomIt, Compare>(first, last, compare);
			}
		}
	}

	template<typename RandomIt, typename Compare>
	void sort(RandomIt first, RandomIt last, Compare predicate)
	{
		simstd::pvt::quick_sort(first, last, predicate);
	}

	template<typename RandomIt>
	void sort(RandomIt first, RandomIt last)
	{
		typedef simstd::less<typename simstd::iterator_traits<RandomIt>::value_type> less;
		simstd::sort<RandomIt, less>(first, last, less());
	}

	template<typename RandomIt, typename Compare>
	void stable_sort(RandomIt first, RandomIt last, Compare compare)
	{
		simstd::pvt::merge_sort<RandomIt, Compare>(first, last, compare);
	}

	template<typename RandomIt>
	void stable_sort(RandomIt first, RandomIt last)
	{
		typedef simstd::less<typename simstd::iterator_traits<RandomIt>::value_type> less;
		simstd::pvt::merge_sort<RandomIt, less>(first, last, less());
	}

	template<typename RandomIt, typename Compare>
	void nth_element(RandomIt first, RandomIt nth, RandomIt last, Compare compare)
	{
		typedef typename iterator_traits<RandomIt>::value_type value_type;

		while ((last - first) > 5) {
			const value_type midValue(simstd::pvt::median<value_type, Compare>(*first, *(first + (last - first) / 2), *(last - 1), compare));
			const RandomIt midPos(simstd::pvt::get_partition<RandomIt, value_type, Compare>(first, last, midValue, compare));

			if (midPos <= nth)
				first = midPos;
			else
				last = midPos;
		}
		simstd::pvt::insertion_sort<RandomIt, Compare>(first, last, compare);
	}

	template<typename RandomIt>
	void nth_element(RandomIt first, RandomIt nth, RandomIt last)
	{
		typedef simstd::less<typename simstd::iterator_traits<RandomIt>::value_type> less;
		simstd::nth_element<RandomIt, less>(first, nth, last, less());
	}

}

#endif
