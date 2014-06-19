#ifndef BASIS_ALGORITHM_BASE_HPP_
#define BASIS_ALGORITHM_BASE_HPP_

#include <basis/sys/base_types.hpp>

namespace simstd {

	template<typename _Tp>
	_Tp* addressof(_Tp& __r) noexcept
	{
		return reinterpret_cast<_Tp*>(&const_cast<char&>(reinterpret_cast<const volatile char&>(__r)));
	}

	namespace pvt {
		template<typename _Tp>
		struct remove_reference {
			typedef _Tp type;
		};

		template<typename _Tp>
		struct remove_reference<_Tp&> {
			typedef _Tp type;
		};

		template<typename _Tp>
		struct remove_reference<_Tp&&> {
			typedef _Tp type;
		};
	}

#if defined(__GNUC__)
	template<typename _Tp>
	constexpr _Tp&& forward(typename pvt::remove_reference<_Tp>::type& __t) noexcept
	{
		return static_cast<_Tp&&>(__t);
	}

	template<typename _Tp>
	constexpr _Tp&& forward(typename pvt::remove_reference<_Tp>::type&& __t) noexcept
	{
		return static_cast<_Tp&&>(__t);
	}

	template<typename _Tp>
	constexpr typename pvt::remove_reference<_Tp>::type&& move(_Tp&& __t) noexcept
	{
		return static_cast<typename pvt::remove_reference<_Tp>::type&&>(__t);
	}
#else
	template<typename _Tp>
	_Tp&& forward(typename pvt::remove_reference<_Tp>::type& __t) noexcept
	{
		return static_cast<_Tp&&>(__t);
	}

	template<typename _Tp>
	_Tp&& forward(typename pvt::remove_reference<_Tp>::type&& __t) noexcept
	{
		return static_cast<_Tp&&>(__t);
	}

	template<typename _Tp>
	typename pvt::remove_reference<_Tp>::type&& move(_Tp&& __t) noexcept
	{
		return static_cast<typename pvt::remove_reference<_Tp>::type&&>(__t);
	}
#endif

	template<typename Type>
	void swap(Type & a, Type & b)
	{
		Type tmp(move(a));
		a = move(b);
		b = move(tmp);
	}

	template<typename Type, size_t Num>
	void swap(Type (&a)[Num], Type (&b)[Num])
	{
		for (size_t n = 0; n < Num; ++n)
			swap(a[n], b[n]);
	}

	template<typename ForwardIt1, typename ForwardIt2>
	void iter_swap(ForwardIt1 a, ForwardIt2 b)
	{
		using simstd::swap;
		swap(*a, *b);
	}

#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

	template<typename T, typename Compare>
	const T& max(const T& a, const T& b, Compare comp)
	{
		return (comp(a, b)) ? b : a;
	}

	template<typename T>
	const T& max(const T& a, const T& b)
	{
		return (a < b) ? b : a;
	}

	template<typename T, typename Compare>
	const T& min(const T& a, const T& b, Compare comp)
	{
		return (comp(b, a)) ? b : a;
	}

	template<typename T>
	const T& min(const T& a, const T& b)
	{
		return (b < a) ? b : a;
	}

	template<typename InputIt1, typename InputIt2>
	bool equal(InputIt1 first1, InputIt1 last1, InputIt2 first2)
	{
		for (; first1 != last1; ++first1, ++first2)
			if (!(*first1 == *first2))
				return false;
		return true;
	}

	template<typename InputIt1, typename InputIt2, typename Compare>
	bool equal(InputIt1 first1, InputIt1 last1, InputIt2 first2, Compare comp)
	{
		for (; first1 != last1; ++first1, ++first2)
			if (!comp(*first1, *first2))
				return false;
		return true;
	}

	template<typename InputIt, typename OutputIt>
	OutputIt copy(InputIt first, InputIt last, OutputIt d_first)
	{
		while (first != last) {
			*d_first = *first;
			++first;
			++d_first;
		}
		return d_first;
	}

	template<typename InputIt, typename OutputIt, typename UnaryPredicate>
	OutputIt copy_if(InputIt first, InputIt last, OutputIt d_first, UnaryPredicate pred)
	{
		while (first != last) {
			if (pred(*first)) {
				*d_first = *first;
				++d_first;
			}
			++first;
		}
		return d_first;
	}

	template<typename InputIt, typename Size, typename OutputIt>
	OutputIt copy_n(InputIt first, Size count, OutputIt result)
	{
		if (count > 0) {
			*result = *first;
			++result;
			for (Size i = 1; i < count; ++i) {
				*result = *++first;
				++result;
			}
		}
		return result;
	}

	template<typename BidirIt1, typename BidirIt2>
	BidirIt2 copy_backward(BidirIt1 first, BidirIt1 last, BidirIt2 d_last)
	{
		while (first != last)
			*(--d_last) = *(--last);
		return d_last;
	}

	template<typename InputIt, typename OutputIt>
	OutputIt move(InputIt first, InputIt last, OutputIt d_first)
	{
		while (first != last) {
			*d_first = simstd::move(*first);
			++first;
			++d_first;
		}
		return d_first;
	}

	template<class BidirIt1, class BidirIt2>
	BidirIt2 move_backward(BidirIt1 first, BidirIt1 last, BidirIt2 d_last)
	{
		while (first != last)
			*(--d_last) = simstd::move(*(--last));
		return d_last;
	}

	template<typename ForwardIt, typename T>
	void fill(ForwardIt first, ForwardIt last, const T& value)
	{
		for (; first != last; ++first)
			*first = value;
	}

	template<typename OutputIt, typename Size, typename T>
	OutputIt fill_n(OutputIt first, Size count, const T& value)
	{
		for (Size i = 0; i < count; ++i) {
			*first = value;
			++first;
		}
		return first;
	}

	template<typename InputIt1, typename InputIt2, typename Compare>
	bool lexicographical_compare(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2, Compare comp)
	{
		for (; (first1 != last1) && (first2 != last2); ++first1, ++first2) {
			if (comp(*first1, *first2))
				return true;
			if (comp(*first2, *first1))
				return false;
		}
		return (first1 == last1) && (first2 != last2);
	}

	template<typename InputIt1, typename InputIt2>
	bool lexicographical_compare(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2)
	{
		for (; (first1 != last1) && (first2 != last2); ++first1, ++first2) {
			if (*first1 < *first2)
				return true;
			if (*first2 < *first1)
				return false;
		}
		return (first1 == last1) && (first2 != last2);
	}

	namespace pvt {
		template<typename Type>
		Type abs(Type a)
		{
			return (a <= 0) ? -a : a;
		}

		template<typename Size>
		Size Log2(Size n)
		{
			int i;
			for (i = 0; n; ++i)
				n >>= 1;
			return i - 1;
		}
	}
}

#endif
