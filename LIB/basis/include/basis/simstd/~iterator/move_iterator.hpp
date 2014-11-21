#ifndef BASIS_ITERATOR_MOVE_ITERATOR_HPP_
#define BASIS_ITERATOR_MOVE_ITERATOR_HPP_

#include <basis/simstd/~iterator/base.hpp>

namespace simstd
{
	template<typename Iterator>
	class move_iterator
	{
		typedef move_iterator<Iterator>                 this_type;
		typedef iterator_traits<Iterator>               traits_type;

	public:
		typedef Iterator                                iterator_type;
		typedef typename traits_type::iterator_category iterator_category;
		typedef typename traits_type::value_type        value_type;
		typedef typename traits_type::difference_type   difference_type;
		typedef Iterator                                pointer;
		typedef value_type&&                            reference;

		move_iterator() : current() {}
		explicit move_iterator(iterator_type ptr) : current(ptr) {}
		move_iterator(const this_type & other) : current(other.current) {}

		template<typename O>
		move_iterator(const move_iterator<O>& other) : current(other.base()) {}

		template<typename O>
		move_iterator<iterator_type>& operator =(const move_iterator<O> & other) {current = other.base(); return *this;}

		iterator_type base() const     {return current;}

		// Forward iterator requirements
		reference  operator * () const {return simstd::move(*current);}
		pointer    operator ->() const {return current;}
		this_type& operator ++()       {++current; return *this;}
		this_type  operator ++(int)    {return this_type(current++);}

		// Bidirectional iterator requirements
		this_type& operator --()       {--current; return *this;}
		this_type  operator --(int)    {return this_type(current--);}

		// Random access iterator requirements
		reference  operator [](const difference_type & n) const {return simstd::move(current[n]);}
		this_type& operator +=(const difference_type & n)       {current += n; return *this;}
		this_type  operator + (const difference_type & n) const {return this_type(current + n);}
		this_type& operator -=(const difference_type & n)       {current -= n; return *this;}
		this_type  operator - (const difference_type & n) const {return this_type(current - n);}

	protected:
		iterator_type current;
	};

	template<typename Type1, typename Type2>
	bool operator ==(const move_iterator<Type1>& lhs, const move_iterator<Type2>& rhs)
	{
		return lhs.base() == rhs.base();
	}

	template<typename Iterator>
	bool operator ==(const move_iterator<Iterator>& lhs, const move_iterator<Iterator>& rhs)
	{
		return lhs.base() == rhs.base();
	}

	template<typename Type1, typename Type2>
	bool operator < (const move_iterator<Type1>& lhs, const move_iterator<Type2>& rhs)
	{
		return lhs.base() < rhs.base();
	}

	template<typename Iterator>
	bool operator <(const move_iterator<Iterator>& lhs, const move_iterator<Iterator>& rhs)
	{
		return lhs.base() < rhs.base();
	}

	template<typename Type1, typename Type2>
	bool operator !=(const move_iterator<Type1>& lhs, const move_iterator<Type2>& rhs)
	{
		return simstd::rel_ops::operator !=(lhs, rhs);
	}

	template<typename Iterator>
	bool operator !=(const move_iterator<Iterator>& lhs, const move_iterator<Iterator>& rhs)
	{
		return simstd::rel_ops::operator !=(lhs, rhs);
	}

	template<typename Type1, typename Type2>
	bool operator <= (const move_iterator<Type1>& lhs, const move_iterator<Type2>& rhs)
	{
		return simstd::rel_ops::operator <=(lhs, rhs);
	}

	template<typename Iterator>
	bool operator <=(const move_iterator<Iterator>& lhs, const move_iterator<Iterator>& rhs)
	{
		return simstd::rel_ops::operator <=(lhs, rhs);
	}

	template<typename Type1, typename Type2>
	bool operator > (const move_iterator<Type1>& lhs, const move_iterator<Type2>& rhs)
	{
		return simstd::rel_ops::operator >(lhs, rhs);
	}

	template<typename Iterator>
	bool operator >(const move_iterator<Iterator>& lhs, const move_iterator<Iterator>& rhs)
	{
		return simstd::rel_ops::operator >(lhs, rhs);
	}

	template<typename Type1, typename Type2>
	bool operator >=(const move_iterator<Type1>& lhs, const move_iterator<Type2>& rhs)
	{
		return simstd::rel_ops::operator >=(lhs, rhs);
	}

	template<typename Iterator>
	bool operator >=(const move_iterator<Iterator>& lhs, const move_iterator<Iterator>& rhs)
	{
		return simstd::rel_ops::operator >=(lhs, rhs);
	}

	template<typename Iterator>
	move_iterator<Iterator> operator +(typename move_iterator<Iterator>::difference_type n, const move_iterator<Iterator>& it)
	{
		return it + n;
	}

	template<typename IteratorL, typename IteratorR>
	auto operator -(const move_iterator<IteratorL>& lhs, const move_iterator<IteratorR>& rhs) -> decltype(lhs.base() - rhs.base())
	{
		return lhs.base() - rhs.base();
	}

	template<typename Iterator>
	auto operator -(const move_iterator<Iterator>& lhs, const move_iterator<Iterator>& rhs) -> decltype(lhs.base() - rhs.base())
	{
		return lhs.base() - rhs.base();
	}

	template<typename Iterator>
	move_iterator<Iterator> make_move_iterator(Iterator it)
	{
		return move_iterator<Iterator>(it);
	}

}

#endif
