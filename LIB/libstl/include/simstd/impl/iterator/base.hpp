#ifndef LIBSTL_ITERATOR_BASE_HPP_
#define LIBSTL_ITERATOR_BASE_HPP_

#include <simstd/types.hpp>

namespace simstd
{
	struct input_iterator_tag
	{
	};

	struct output_iterator_tag
	{
	};

	struct forward_iterator_tag : public input_iterator_tag
	{
	};


	struct bidirectional_iterator_tag : public forward_iterator_tag
	{
	};


	struct random_access_iterator_tag : public bidirectional_iterator_tag
	{
	};

	template <typename Iterator>
	struct iterator_traits
	{
		typedef typename Iterator::iterator_category iterator_category;
		typedef typename Iterator::value_type        value_type;
		typedef typename Iterator::difference_type   difference_type;
		typedef typename Iterator::pointer           pointer;
		typedef typename Iterator::reference         reference;
	};

	template <typename Type>
	struct iterator_traits<Type*>
	{
		typedef random_access_iterator_tag iterator_category;
		typedef Type                       value_type;
		typedef ptrdiff_t                  difference_type;
		typedef Type*                      pointer;
		typedef Type&                      reference;
	};

	template <typename Type>
	struct iterator_traits<const Type*>
	{
		typedef random_access_iterator_tag iterator_category;
		typedef Type                       value_type;
		typedef ptrdiff_t                  difference_type;
		typedef const Type*                pointer;
		typedef const Type&                reference;
	};

	namespace pvt {
		template<typename Iterator>
		typename iterator_traits<Iterator>::iterator_category iterator_category(const Iterator &)
		{
			return typename iterator_traits<Iterator>::iterator_category();
		}

		template<typename InputIterator>
		typename iterator_traits<InputIterator>::difference_type distance(InputIterator first, InputIterator last, input_iterator_tag)
		{
			typename iterator_traits<InputIterator>::difference_type n = 0;
			while (first != last)
			{
				++first;
				++n;
			}
			return n;
		}

		template<typename RandomIterator>
		typename iterator_traits<RandomIterator>::difference_type distance(RandomIterator first, RandomIterator last, random_access_iterator_tag)
		{
			return last - first;
		}

		template<typename InputIterator, typename Distance>
		void advance(InputIterator& current, Distance n, input_iterator_tag)
		{
			while (n--)
				++current;
		}

		template<typename BidirIterator, typename Distance>
		void advance(BidirIterator & current, Distance n, bidirectional_iterator_tag)
		{
			if (n > 0)
				while (n--)
					++current;
			else
				while (n++)
					--current;
		}

		template<typename RandomIterator, typename Distance>
		void advance(RandomIterator & current, Distance n, random_access_iterator_tag)
		{
			current += n;
		}
	}

	template<typename InputIterator>
	typename iterator_traits<InputIterator>::difference_type distance(InputIterator first, InputIterator last)
	{
		return simstd::pvt::distance(first, last, simstd::pvt::iterator_category(first));
	}

	template<typename InputIterator, typename Distance>
	void advance(InputIterator & current, Distance n)
	{
		typename iterator_traits<InputIterator>::difference_type d = n;
		simstd::pvt::advance(current, d, simstd::pvt::iterator_category(current));
	}

	template<typename ForwardIterator>
	ForwardIterator next(ForwardIterator current, typename iterator_traits<ForwardIterator>::difference_type n = 1)
	{
		advance(current, n);
		return current;
	}

	template<typename BidirIterator>
	BidirIterator prev(BidirIterator current, typename iterator_traits<BidirIterator>::difference_type n = 1)
	{
		advance(current, -n);
		return current;
	}

	///=============================================================================================
	template<
		typename Category,
		typename Type,
		typename Distance = ptrdiff_t,
		typename Pointer = Type*,
		typename Reference = Type&>
	struct iterator
	{
		typedef Category  iterator_category;
		typedef Type      value_type;
		typedef Distance  difference_type;
		typedef Pointer   pointer;
		typedef Reference reference;
	};
}

#endif
