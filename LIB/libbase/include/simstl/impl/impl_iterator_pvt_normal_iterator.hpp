#ifndef SARALIB_STL_ITERATOR_NORMAL_ITERATOR_HPP_
#define SARALIB_STL_ITERATOR_NORMAL_ITERATOR_HPP_

#include "impl_types.hpp"
#include "impl_iterator_base.hpp"
#include "impl_rel_ops.hpp"

namespace sarastd
{
	namespace pvt {
		template<typename Pointer>
		class _normal_iterator
		{
			typedef Pointer                                 iterator_type;
			typedef _normal_iterator<Pointer>               this_type;
			typedef iterator_traits<Pointer>                traits_type;

		public:
			typedef typename traits_type::iterator_category iterator_category;
			typedef typename traits_type::value_type        value_type;
			typedef typename traits_type::difference_type   difference_type;
			typedef typename traits_type::pointer           pointer;
			typedef typename traits_type::reference         reference;

			_normal_iterator() : current() {}
			explicit _normal_iterator(iterator_type ptr) : current(ptr) {}
			_normal_iterator(const this_type & other) : current(other.current) {}

			template <typename O>
			_normal_iterator(const _normal_iterator<O>& other) : current(other.base()) {}

			template <typename O>
			_normal_iterator<iterator_type>& operator =(const _normal_iterator<O> & other) {current = other.base(); return *this;}

			iterator_type base() const     {return current;}

			// Forward iterator requirements
			reference  operator * () const {return *current;}
			pointer    operator ->() const {return current;}
			this_type& operator ++()       {++current; return *this;}
			this_type  operator ++(int)    {return this_type(current++);}

			// Bidirectional iterator requirements
			this_type& operator --()       {--current; return *this;}
			this_type  operator --(int)    {return this_type(current--);}

			// Random access iterator requirements
			reference  operator [](const difference_type & n) const {return current[n];}
			this_type& operator +=(const difference_type & n)       {current += n; return *this;}
			this_type  operator + (const difference_type & n) const {return this_type(current + n);}
			this_type& operator -=(const difference_type & n)       {current -= n; return *this;}
			this_type  operator - (const difference_type & n) const {return this_type(current - n);}

		protected:
			iterator_type current;
		};

		template<typename Type1, typename Type2>
		bool operator ==(const _normal_iterator<Type1>& lhs, const _normal_iterator<Type2>& rhs)
		{
			return lhs.base() == rhs.base();
		}

		template<typename Type1, typename Type2>
		bool operator < (const _normal_iterator<Type1>& lhs, const _normal_iterator<Type2>& rhs)
		{
			return lhs.base() < rhs.base();
		}

		template<typename Type1, typename Type2>
		bool operator !=(const _normal_iterator<Type1>& lhs, const _normal_iterator<Type2>& rhs)
		{
			return sarastd::rel_ops::operator !=(lhs, rhs);
		}

		template<typename Type1, typename Type2>
		bool operator <= (const _normal_iterator<Type1>& lhs, const _normal_iterator<Type2>& rhs)
		{
			return sarastd::rel_ops::operator <=(lhs, rhs);
		}

		template<typename Type1, typename Type2>
		bool operator > (const _normal_iterator<Type1>& lhs, const _normal_iterator<Type2>& rhs)
		{
			return sarastd::rel_ops::operator >(lhs, rhs);
		}

		template<typename Type1, typename Type2>
		bool operator >=(const _normal_iterator<Type1>& lhs, const _normal_iterator<Type2>& rhs)
		{
			return sarastd::rel_ops::operator >=(lhs, rhs);
		}

		template<typename Type>
		_normal_iterator<Type> operator +(typename _normal_iterator<Type>::difference_type n, const _normal_iterator<Type> & it)
		{
			return it + n;
		}

		template<typename Type>
		typename
		_normal_iterator<Type>::difference_type operator -(const _normal_iterator<Type>& lhs, const _normal_iterator<Type>& rhs)
		{
			return lhs.base() - rhs.base();
		}
	}
}

#endif
