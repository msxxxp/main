#ifndef SARALIB_STL_ALGORITHM_HEAP_HPP_
#define SARALIB_STL_ALGORITHM_HEAP_HPP_

#include "impl_types.hpp"
#include "impl_algorithm_base.hpp"
#include "impl_functional.hpp"
#include "impl_iterator.hpp"
#include "impl_utility.hpp"

namespace sarastd {

	namespace pvt {
		template<typename RandomIt, typename Distance, typename T, typename Compare>
		void promote_heap(RandomIt first, Distance topPosition, Distance position, const T& value, Compare compare)
		{
			for (Distance parentPosition = (position - 1) >> 1; // This formula assumes that (position > 0). // We use '>> 1' instead of '/ 2' because we have seen VC++ generate better code with >>.
			(position > topPosition) && compare(*(first + parentPosition), value); parentPosition = (position - 1) >> 1) {
				*(first + position) = *(first + parentPosition); // Swap the node with its parent.
				position = parentPosition;
			}

			*(first + position) = value;
		}

		template<typename RandomIt, typename Distance, typename T, typename Compare>
		void adjust_heap(RandomIt first, Distance topPosition, Distance heapSize, Distance position, const T& value, Compare compare)
		{
			Distance childPosition = (2 * position) + 2;

			for (; childPosition < heapSize; childPosition = (2 * childPosition) + 2) {
				if (compare(*(first + childPosition), *(first + (childPosition - 1)))) // Choose the larger of the two children.
					--childPosition;
				*(first + position) = *(first + childPosition); // Swap positions with this child.
				position = childPosition;
			}

			if (childPosition == heapSize) // If we are at the bottom...
			    {
				*(first + position) = *(first + (childPosition - 1));
				position = childPosition - 1;
			}

			sarastd::pvt::promote_heap<RandomIt, Distance, T, Compare>(first, topPosition, position, value, compare);
		}

		template<typename RandomIt, typename Distance, typename Compare>
		void remove_heap(RandomIt first, Distance heapSize, Distance position, Compare compare)
		{
			typedef typename sarastd::iterator_traits<RandomIt>::difference_type difference_type;
			typedef typename sarastd::iterator_traits<RandomIt>::value_type value_type;

			const value_type tempBottom(*(first + heapSize - 1));
			*(first + heapSize - 1) = *(first + position);
			sarastd::pvt::adjust_heap<RandomIt, difference_type, value_type, Compare>(first, (difference_type)0, (difference_type)(heapSize - 1), (difference_type)position, tempBottom,
			                                                                                      compare);
		}

		template<typename RandomIt, typename Distance, typename Compare>
		void change_heap(RandomIt first, Distance heapSize, Distance position, Compare compare)
		{
			typedef typename sarastd::iterator_traits<RandomIt>::difference_type difference_type;
			typedef typename sarastd::iterator_traits<RandomIt>::value_type value_type;

			sarastd::pvt::remove_heap<RandomIt, Distance, Compare>(first, heapSize, position, compare);

			value_type tempBottom(*(first + heapSize - 1));

			sarastd::pvt::promote_heap<RandomIt, difference_type, value_type, Compare>(first, (difference_type)0, (difference_type)(heapSize - 1), tempBottom, compare);
		}
	}

	template<typename RandomIt, typename Compare>
	bool is_heap(RandomIt first, RandomIt last, Compare compare)
	{
		int counter = 0;

		for (RandomIt child = first + 1; child < last; ++child, counter ^= 1) {
			if (compare(*first, *child))
				return false;
			first += counter; // counter switches between 0 and 1 every time through.
		}
		return true;
	}

	template<typename RandomIt>
	bool is_heap(RandomIt first, RandomIt last)
	{
		typedef sarastd::less<typename sarastd::iterator_traits<RandomIt>::value_type> less;
		return sarastd::is_heap<RandomIt, less>(first, last, less());
	}

	template<typename RandomIt, typename Compare>
	void make_heap(RandomIt first, RandomIt last, Compare compare)
	{
		typedef typename sarastd::iterator_traits<RandomIt>::difference_type difference_type;
		typedef typename sarastd::iterator_traits<RandomIt>::value_type value_type;

		const difference_type heapSize = last - first;

		if (heapSize >= 2) // If there is anything to do... (we need this check because otherwise the math fails below).
		    {
			difference_type parentPosition = ((heapSize - 2) >> 1) + 1; // We use '>> 1' instead of '/ 2' because we have seen VC++ generate better code with >>.

			do {
				--parentPosition;
				const value_type temp(*(first + parentPosition));
				sarastd::pvt::adjust_heap<RandomIt, difference_type, value_type, Compare>(first, parentPosition, heapSize, parentPosition, temp, compare);
			} while (parentPosition != 0);
		}
	}

	template<typename RandomIt>
	void make_heap(RandomIt first, RandomIt last)
	{
		typedef sarastd::less<typename sarastd::iterator_traits<RandomIt>::value_type> less;
		sarastd::make_heap<RandomIt, less>(first, last, less());
	}

	template<typename RandomIt, typename Compare>
	void push_heap(RandomIt first, RandomIt last, Compare compare)
	{
		typedef typename sarastd::iterator_traits<RandomIt>::difference_type difference_type;
		typedef typename sarastd::iterator_traits<RandomIt>::value_type value_type;

		const value_type tempBottom(*(last - 1));

		sarastd::pvt::promote_heap<RandomIt, difference_type, value_type, Compare>(first, (difference_type)0, (difference_type)(last - first - 1), tempBottom, compare);
	}

	template<typename RandomIt>
	void push_heap(RandomIt first, RandomIt last)
	{
		typedef sarastd::less<typename sarastd::iterator_traits<RandomIt>::value_type> less;
		sarastd::push_heap<RandomIt, less>(first, last, less());
	}

	template<typename RandomIt, typename Compare>
	void pop_heap(RandomIt first, RandomIt last, Compare compare)
	{
		typedef typename sarastd::iterator_traits<RandomIt>::difference_type difference_type;
		typedef typename sarastd::iterator_traits<RandomIt>::value_type value_type;

		const value_type tempBottom(*(last - 1));
		*(last - 1) = *first;
		sarastd::pvt::adjust_heap<RandomIt, difference_type, value_type, Compare>(first, (difference_type)0, (difference_type)(last - first - 1), 0, tempBottom, compare);
	}

	template<typename RandomIt>
	void pop_heap(RandomIt first, RandomIt last)
	{
		typedef sarastd::less<typename sarastd::iterator_traits<RandomIt>::value_type> less;
		sarastd::pop_heap<RandomIt, less>(first, last, less());
	}

	template<typename RandomIt, typename Compare>
	void sort_heap(RandomIt first, RandomIt last, Compare compare)
	{
		for (; (last - first) > 1; --last)
			sarastd::pop_heap<RandomIt, Compare>(first, last, compare);
	}

	template<typename RandomIt>
	void sort_heap(RandomIt first, RandomIt last)
	{
		typedef sarastd::less<typename sarastd::iterator_traits<RandomIt>::value_type> less;
		sarastd::sort_heap<RandomIt, less>(first, last, less());
	}
}

#endif
