#ifndef SARALIB_STL_ITERATOR_REVERSE_ITERATOR_HPP_
#define SARALIB_STL_ITERATOR_REVERSE_ITERATOR_HPP_

#include "impl_types.hpp"
#include "impl_iterator_base.hpp"

namespace sarastd
{
	template<typename Iterator>
	class reverse_iterator
	{
		typedef Iterator                                iterator_type;
		typedef reverse_iterator<Iterator>              this_type;
		typedef iterator_traits<Iterator>               traits_type;

	public:
		typedef typename traits_type::iterator_category iterator_category;
		typedef typename traits_type::value_type        value_type;
		typedef typename traits_type::difference_type   difference_type;
		typedef typename traits_type::reference         reference;
		typedef typename traits_type::pointer           pointer;

		reverse_iterator() : current() {}
		explicit reverse_iterator(Iterator it) : current(it) {}
		reverse_iterator(const this_type& ri) : current(ri.current) {}

		template <typename U>
		reverse_iterator(const reverse_iterator<U>& ri) : current(ri.base()) {}

		template <typename U>
		reverse_iterator<Iterator>& operator =(const reverse_iterator<U>& ri) {current = ri.base(); return *this;}

		iterator_type base() const                         {return current;}

		// Forward iterator requirements
		reference     operator * () const                  {iterator_type tmp = current; return *--tmp;}
		pointer       operator ->() const                  {return &(operator*());}
		this_type&    operator ++()                        {--current; return *this;}
		this_type     operator ++(int)                     {this_type tmp = *this; --current; return tmp;}

		// Bidirectional iterator requirements
		this_type&    operator --()                        {++current; return *this;}
		this_type     operator --(int)                     {this_type tmp = *this; ++current; return tmp;}

		// Random access iterator requirements
		reference     operator [](difference_type n) const {return *(*this + n);}
		this_type     operator +(difference_type n) const  {return this_type(current - n);}
		this_type&    operator +=(difference_type n)       {current -= n; return *this;}
		this_type     operator -(difference_type n) const  {return this_type(current + n);}
		this_type&    operator -=(difference_type n)       {current += n; return *this;}

	protected:
		iterator_type current;
	};

	template<typename Iterator1, typename Iterator2>
	bool operator ==(const reverse_iterator<Iterator1>& lhs, const reverse_iterator<Iterator2>& rhs)
	{
		return lhs.base() == rhs.base();
	}

	template<typename Iterator1, typename Iterator2>
	bool operator !=(const reverse_iterator<Iterator1>& lhs, const reverse_iterator<Iterator2>& rhs)
	{
		return sarastd::pvt::ops::operator !=(lhs, rhs);
	}

	template<typename Iterator1, typename Iterator2>
	bool operator <(const reverse_iterator<Iterator1>& lhs, const reverse_iterator<Iterator2>& rhs)
	{
		return lhs.base() < rhs.base();
	}

	template<typename Iterator1, typename Iterator2>
	bool operator <=(const reverse_iterator<Iterator1>& lhs, const reverse_iterator<Iterator2>& rhs)
	{
		return sarastd::pvt::ops::operator <=(lhs, rhs);
	}

	template<typename Iterator1, typename Iterator2>
	bool operator >(const reverse_iterator<Iterator1>& lhs, const reverse_iterator<Iterator2>& rhs)
	{
		return sarastd::pvt::ops::operator >(lhs, rhs);
	}

	template<typename Iterator1, typename Iterator2>
	bool operator >=(const reverse_iterator<Iterator1>& lhs, const reverse_iterator<Iterator2>& rhs)
	{
		return sarastd::pvt::ops::operator >=(lhs, rhs);
	}

	template<typename Iterator>
	reverse_iterator<Iterator> operator +(typename reverse_iterator<Iterator>::difference_type n, const reverse_iterator<Iterator> & it)
	{
		return it + n;
	}

	template<typename Iterator>
	typename
	reverse_iterator<Iterator>::difference_type operator -(const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs)
	{
		return rhs.base() - lhs.base();
	}
}

#endif
