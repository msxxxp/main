#ifndef LIBSTL_ITERATOR_CONST_VALUE_ITERATOR_HPP_
#define LIBSTL_ITERATOR_CONST_VALUE_ITERATOR_HPP_

#include "impl_types.hpp"
#include "impl_iterator_base.hpp"

namespace simstd
{
	namespace pvt {
		template<typename Iterator>
		class _const_value_iterator : public iterator<random_access_iterator_tag, Iterator>
		{
			typedef _const_value_iterator<Iterator>         this_type;

		public:
			typedef Iterator                                iterator_type;
			typedef iterator_traits<Iterator>               traits_type;
			typedef typename traits_type::iterator_category iterator_category;
			typedef typename traits_type::value_type        value_type;
			typedef typename traits_type::difference_type   difference_type;
			typedef typename traits_type::reference         reference;
			typedef typename traits_type::pointer           pointer;
			typedef simstd::size_t size_type;

			_const_value_iterator(iterator_type v, iterator_type it): value(v), current(it) {}

			iterator_type base() const        {return current;}

			reference     operator * () const {return *value;}
			pointer       operator ->() const {return value;}
			this_type&    operator ++()       {++current; return *this;}
			this_type     operator ++(int)    {this_type tmp = *this; ++current; return tmp;}

			// Bidirectional iterator requirements
			this_type&    operator --()       {--current; return *this;}
			this_type     operator --(int)    {this_type tmp = *this; --current; return tmp;}

			// Random access iterator requirements
			reference     operator [](difference_type n) const {return *value;}
			this_type     operator +(difference_type n) const  {return this_type(value, current + n);}
			this_type&    operator +=(difference_type n)       {current += n; return *this;}
			this_type     operator -(difference_type n) const  {return this_type(value, current - n);}
			this_type&    operator -=(difference_type n)       {current -= n; return *this;}

		private:
			iterator_type value;
			iterator_type current;
		};

		template<typename Iterator>
		bool operator ==(const _const_value_iterator<Iterator>& lhs, const _const_value_iterator<Iterator>& rhs)
		{
			return lhs.base() == rhs.base();
		}

		template<typename Iterator>
		bool operator !=(const _const_value_iterator<Iterator>& lhs, const _const_value_iterator<Iterator>& rhs)
		{
			return simstd::rel_ops::operator !=(lhs, rhs);
		}

		template<typename Iterator>
		bool operator <(const _const_value_iterator<Iterator>& lhs, const _const_value_iterator<Iterator>& rhs)
		{
			return lhs.base() < rhs.base();
		}

		template<typename Iterator>
		bool operator <=(const _const_value_iterator<Iterator>& lhs, const _const_value_iterator<Iterator>& rhs)
		{
			return simstd::rel_ops::operator <=(lhs, rhs);
		}

		template<typename Iterator>
		bool operator >(const _const_value_iterator<Iterator>& lhs, const _const_value_iterator<Iterator>& rhs)
		{
			return simstd::rel_ops::operator >(lhs, rhs);
		}

		template<typename Iterator>
		bool operator >=(const _const_value_iterator<Iterator>& lhs, const _const_value_iterator<Iterator>& rhs)
		{
			return simstd::rel_ops::operator >=(lhs, rhs);
		}

		template<typename Iterator>
		_const_value_iterator<Iterator> operator +(typename _const_value_iterator<Iterator>::difference_type n, const _const_value_iterator<Iterator> & it)
		{
			return it + n;
		}

		template<typename Iterator>
		typename
		_const_value_iterator<Iterator>::difference_type operator -(const _const_value_iterator<Iterator>& lhs, const _const_value_iterator<Iterator>& rhs)
		{
			return lhs.base() - rhs.base();
		}

		template<typename Type>
		class _value_generator
		{
			typedef _value_generator<Type> this_type;
			typedef Type value_type;
			typedef simstd::size_t size_type;
			typedef const Type* const_pointer;
			typedef _const_value_iterator<const_pointer> const_iterator;

		public:
			explicit _value_generator(size_type n, const value_type & v) :
				value(v),
				begin_(0),
				end_(begin_ + n)
			{
			}
			_value_generator(const this_type & other) :
				value(other.value),
				begin_(other.begin_),
				end_(other.end_)
			{
			}

			const_iterator begin() const
			{
				return const_iterator(&value, begin_);
			}
			const_iterator end() const
			{
				return const_iterator(&value, end_);
			}

		private:
			value_type value;
			const_pointer begin_;
			const_pointer end_;
		};
	}
}

#endif
