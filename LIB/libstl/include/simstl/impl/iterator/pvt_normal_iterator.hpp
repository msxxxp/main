#ifndef LIBSTL_ITERATOR_NORMAL_ITERATOR_HPP_
#define LIBSTL_ITERATOR_NORMAL_ITERATOR_HPP_

#include <simstl/impl/iterator/base.hpp>
#include <simstl/impl/utility/rel_ops.hpp>

namespace simstd
{
	namespace pvt {
		template<typename Pointer>
		class normal_iterator
		{
			typedef Pointer                                 iterator_type;
			typedef normal_iterator<Pointer>                this_type;
			typedef iterator_traits<Pointer>                traits_type;

		public:
			typedef typename traits_type::iterator_category iterator_category;
			typedef typename traits_type::value_type        value_type;
			typedef typename traits_type::difference_type   difference_type;
			typedef typename traits_type::pointer           pointer;
			typedef typename traits_type::reference         reference;

			normal_iterator() : current() {}
			explicit normal_iterator(iterator_type ptr) : current(ptr) {}
			normal_iterator(const this_type & other) : current(other.current) {}

			template <typename O>
			normal_iterator(const normal_iterator<O>& other) : current(other.base()) {}

			template <typename O>
			normal_iterator<iterator_type>& operator =(const normal_iterator<O> & other) {current = other.base(); return *this;}

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
		bool operator ==(const normal_iterator<Type1>& lhs, const normal_iterator<Type2>& rhs)
		{
			return lhs.base() == rhs.base();
		}

		template<typename Type1, typename Type2>
		bool operator < (const normal_iterator<Type1>& lhs, const normal_iterator<Type2>& rhs)
		{
			return lhs.base() < rhs.base();
		}

		template<typename Type1, typename Type2>
		bool operator !=(const normal_iterator<Type1>& lhs, const normal_iterator<Type2>& rhs)
		{
			return simstd::rel_ops::operator !=(lhs, rhs);
		}

		template<typename Type1, typename Type2>
		bool operator <= (const normal_iterator<Type1>& lhs, const normal_iterator<Type2>& rhs)
		{
			return simstd::rel_ops::operator <=(lhs, rhs);
		}

		template<typename Type1, typename Type2>
		bool operator > (const normal_iterator<Type1>& lhs, const normal_iterator<Type2>& rhs)
		{
			return simstd::rel_ops::operator >(lhs, rhs);
		}

		template<typename Type1, typename Type2>
		bool operator >=(const normal_iterator<Type1>& lhs, const normal_iterator<Type2>& rhs)
		{
			return simstd::rel_ops::operator >=(lhs, rhs);
		}

		template<typename Type>
		normal_iterator<Type> operator +(typename normal_iterator<Type>::difference_type n, const normal_iterator<Type> & it)
		{
			return it + n;
		}

		template<typename Type>
		typename
		normal_iterator<Type>::difference_type operator -(const normal_iterator<Type>& lhs, const normal_iterator<Type>& rhs)
		{
			return lhs.base() - rhs.base();
		}
	}
}

#endif
