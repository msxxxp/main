#ifndef LIBSTL_VECTOR_HPP_
#define LIBSTL_VECTOR_HPP_

#include "impl_types.hpp"
#include "impl_algorithm_base.hpp"
#include "impl_iterator_base.hpp"
#include "impl_iterator_back_insert_iterator.hpp"
#include "impl_iterator_insert_iterator.hpp"
#include "impl_iterator_pvt_normal_iterator.hpp"
#include "impl_iterator_pvt_const_value_iterator.hpp"
#include "impl_iterator_reverse_iterator.hpp"
#include "impl_memory.hpp"
#include "impl_vector_impl.hpp"

namespace simstd {
	namespace Test {
		ssize_t _vector(aPrintFunc printFunc);
	}

	template<typename Type, typename Allocator = simstd::allocator<Type> >
	class vector {
		typedef vector this_type;

	public:
		typedef Type value_type;
		typedef Allocator allocator_type;
		typedef simstd::size_t size_type;
		typedef allocator_traits<allocator_type> alloc_traits;
		typedef typename alloc_traits::pointer pointer;
		typedef typename alloc_traits::const_pointer const_pointer;
		typedef typename alloc_traits::reference reference;
		typedef typename alloc_traits::const_reference const_reference;
		typedef typename alloc_traits::difference_type difference_type;
		typedef simstd::pvt::normal_iterator<pointer> iterator;
		typedef simstd::pvt::normal_iterator<const_pointer> const_iterator;
		typedef simstd::reverse_iterator<iterator> reverse_iterator;
		typedef simstd::reverse_iterator<const_iterator> const_reverse_iterator;

	public:
		~vector();

		vector();
		explicit vector(size_type n);
		vector(size_type n, const value_type& val);
		vector(const this_type& other);

		template<typename InputIterator>
		vector(InputIterator first, InputIterator last);

		this_type& operator =(const this_type& other);

		template<typename InputIterator>
		void assign(InputIterator first, InputIterator last);
		void assign(size_type n, const value_type& value);

		reference       operator [](size_type pos);
		const_reference operator [](size_type pos) const;
		reference       at(size_type pos);
		const_reference at(size_type pos) const;

		reference         front();
		const_reference   front() const;
		reference         back();
		const_reference   back() const;
		value_type*       data();
		const value_type* data() const;

		iterator               begin();
		const_iterator         begin() const;
		const_iterator         cbegin() const;
		iterator               end();
		const_iterator         end() const;
		const_iterator         cend() const;
		reverse_iterator       rbegin();
		const_reverse_iterator rbegin() const;
		const_reverse_iterator crbegin() const;
		reverse_iterator       rend();
		const_reverse_iterator rend() const;
		const_reverse_iterator crend() const;

		bool      empty() const;
		size_type size() const;
		size_type max_size() const;
		void      reserve(size_type size);
		size_type capacity() const;
		void      shrink_to_fit();

		void      clear();

		template<typename InputIterator>
		iterator insert(const_iterator pos, InputIterator first, InputIterator last);
		iterator insert(const_iterator pos, const value_type& value);
		iterator insert(const_iterator pos, size_type n, const value_type& value);

		iterator erase(const_iterator first, const_iterator last);
		iterator erase(const_iterator pos);

		void     push_back(const value_type& value);
		void     pop_back();

		void     resize(size_type count, const value_type& value);
		void     resize(size_type count);

		void     swap(this_type& other);

	protected:
		typedef simstd::pvt::vector_impl<value_type, allocator_type> impl_type;
		impl_type m_impl;

	private:
		void _resize_increase(size_type count, const value_type& value);

		template<typename InputIterator>
		void _insert_back(InputIterator first, InputIterator last, simstd::input_iterator_tag);

		template<typename ForwardIterator>
		void _insert_back(ForwardIterator first, ForwardIterator last, simstd::forward_iterator_tag);

		template<typename InputIterator>
		iterator _insert(const_iterator pos, InputIterator first, InputIterator last, simstd::input_iterator_tag);

		template<typename ForwardIterator>
		iterator _insert(const_iterator pos, ForwardIterator first, ForwardIterator last, simstd::forward_iterator_tag);
	};

	///=============================================================================================
	template<typename Type, typename Allocator>
	bool operator ==(const vector<Type, Allocator>& lhs, const vector<Type, Allocator>& rhs)
	{
		return (lhs.size() == rhs.size() && simstd::equal(lhs.begin(), lhs.end(), rhs.begin()));
	}

	template<typename Type, typename Allocator>
	bool operator !=(const vector<Type, Allocator>& lhs, const vector<Type, Allocator>& rhs)
	{
		return simstd::rel_ops::operator !=(lhs, rhs);
	}

	template<typename Type, typename Allocator>
	bool operator <(const vector<Type, Allocator>& lhs, const vector<Type, Allocator>& rhs)
	{
		return simstd::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
	}

	template<typename Type, typename Allocator>
	bool operator >(const vector<Type, Allocator>& lhs, const vector<Type, Allocator>& rhs)
	{
		return simstd::rel_ops::operator >(lhs, rhs);
	}

	template<typename Type, typename Allocator>
	bool operator <=(const vector<Type, Allocator>& lhs, const vector<Type, Allocator>& rhs)
	{
		return simstd::rel_ops::operator <=(lhs, rhs);
	}

	template<typename Type, typename Allocator>
	bool operator >=(const vector<Type, Allocator>& lhs, const vector<Type, Allocator>& rhs)
	{
		return simstd::rel_ops::operator >=(lhs, rhs);
	}

	template<typename Type, typename Allocator>
	void swap(vector<Type, Allocator>& lhs, vector<Type, Allocator>& rhs)
	{
		lhs.swap(rhs);
	}

	///=============================================================================================
	template<typename Type, typename Allocator>
	vector<Type, Allocator>::~vector()
	{
	}

	template<typename Type, typename Allocator>
	vector<Type, Allocator>::vector() :
		m_impl()
	{
	}

	template<typename Type, typename Allocator>
	vector<Type, Allocator>::vector(size_type n) :
		m_impl()
	{
		_resize_increase(n, value_type());
	}

	template<typename Type, typename Allocator>
	vector<Type, Allocator>::vector(size_type n, const value_type& value) :
		m_impl()
	{
		_resize_increase(n, value);
	}

	template<typename Type, typename Allocator>
	template<typename InputIterator>
	vector<Type, Allocator>::vector(InputIterator first, InputIterator last) :
		m_impl()
	{
		_insert_back(first, last, simstd::pvt::iterator_category(first));
	}

	template<typename Type, typename Allocator>
	vector<Type, Allocator>::vector(const this_type& other) :
		m_impl(other.capacity(), other.m_impl.begin, other.m_impl.end)
	{
	}

	template<typename Type, typename Allocator>
	typename
	vector<Type, Allocator>::this_type& vector<Type, Allocator>::operator =(const this_type& other)
	{
		this_type(other).swap(*this);
		return *this;
	}

	template<typename Type, typename Allocator>
	void vector<Type, Allocator>::assign(size_type n, const value_type& value)
	{
		this_type(n, value).swap(*this);
	}

	template<typename Type, typename Allocator>
	template<typename InputIterator>
	void vector<Type, Allocator>::assign(InputIterator first, InputIterator last)
	{
		this_type(first, last).swap(*this);
	}

	template<typename Type, typename Allocator>
	typename
	vector<Type, Allocator>::reference vector<Type, Allocator>::operator [](size_type pos)
	{
		return *(m_impl.begin + pos);
	}

	template<typename Type, typename Allocator>
	typename
	vector<Type, Allocator>::const_reference vector<Type, Allocator>::operator [](size_type pos) const
	{
		return *(m_impl.begin + pos);
	}

	template<typename Type, typename Allocator>
	typename
	vector<Type, Allocator>::reference vector<Type, Allocator>::at(size_type pos)
	{
		return *(m_impl.begin + pos);
	}

	template<typename Type, typename Allocator>
	typename
	vector<Type, Allocator>::const_reference vector<Type, Allocator>::at(size_type pos) const
	{
		return *(m_impl.begin + pos);
	}

	template<typename Type, typename Allocator>
	typename
	vector<Type, Allocator>::reference vector<Type, Allocator>::front()
	{
		return *(m_impl.begin);
	}

	template<typename Type, typename Allocator>
	typename
	vector<Type, Allocator>::const_reference vector<Type, Allocator>::front() const
	{
		return *(m_impl.begin);
	}

	template<typename Type, typename Allocator>
	typename
	vector<Type, Allocator>::reference vector<Type, Allocator>::back()
	{
		return *(m_impl.end - 1);
	}

	template<typename Type, typename Allocator>
	typename
	vector<Type, Allocator>::const_reference vector<Type, Allocator>::back() const
	{
		return *(m_impl.end - 1);
	}

	template<typename Type, typename Allocator>
	typename
	vector<Type, Allocator>::value_type* vector<Type, Allocator>::data()
	{
		return m_impl.begin;
	}

	template<typename Type, typename Allocator>
	const typename vector<Type, Allocator>::value_type* vector<Type, Allocator>::data() const
	{
		return m_impl.begin;
	}

	template<typename Type, typename Allocator>
	typename
	vector<Type, Allocator>::iterator vector<Type, Allocator>::begin()
	{
		return iterator(m_impl.begin);
	}

	template<typename Type, typename Allocator>
	typename
	vector<Type, Allocator>::const_iterator vector<Type, Allocator>::begin() const
	{
		return cbegin();
	}

	template<typename Type, typename Allocator>
	typename
	vector<Type, Allocator>::const_iterator vector<Type, Allocator>::cbegin() const
	{
		return const_iterator(m_impl.begin);
	}

	template<typename Type, typename Allocator>
	typename
	vector<Type, Allocator>::iterator vector<Type, Allocator>::end()
	{
		return iterator(m_impl.end);
	}

	template<typename Type, typename Allocator>
	typename
	vector<Type, Allocator>::const_iterator vector<Type, Allocator>::end() const
	{
		return cend();
	}

	template<typename Type, typename Allocator>
	typename
	vector<Type, Allocator>::const_iterator vector<Type, Allocator>::cend() const
	{
		return const_iterator(m_impl.end);
	}

	template<typename Type, typename Allocator>
	typename
	vector<Type, Allocator>::reverse_iterator vector<Type, Allocator>::rbegin()
	{
		return reverse_iterator(iterator(m_impl.end));
	}

	template<typename Type, typename Allocator>
	typename
	vector<Type, Allocator>::const_reverse_iterator vector<Type, Allocator>::rbegin() const
	{
		return crbegin();
	}

	template<typename Type, typename Allocator>
	typename
	vector<Type, Allocator>::const_reverse_iterator vector<Type, Allocator>::crbegin() const
	{
		return const_reverse_iterator(const_iterator(m_impl.end));
	}

	template<typename Type, typename Allocator>
	typename
	vector<Type, Allocator>::reverse_iterator vector<Type, Allocator>::rend()
	{
		return reverse_iterator(iterator(m_impl.begin));
	}

	template<typename Type, typename Allocator>
	typename
	vector<Type, Allocator>::const_reverse_iterator vector<Type, Allocator>::rend() const
	{
		return crend();
	}

	template<typename Type, typename Allocator>
	typename
	vector<Type, Allocator>::const_reverse_iterator vector<Type, Allocator>::crend() const
	{
		return const_reverse_iterator(const_iterator(m_impl.begin));
	}

	template<typename Type, typename Allocator>
	bool vector<Type, Allocator>::empty() const
	{
		return m_impl.end == m_impl.begin;
	}

	template<typename Type, typename Allocator>
	typename
	vector<Type, Allocator>::size_type vector<Type, Allocator>::size() const
	{
		return m_impl.size();
	}

	template<typename Type, typename Allocator>
	typename
	vector<Type, Allocator>::size_type vector<Type, Allocator>::max_size() const
	{
		return m_impl.max_size();
	}

	template<typename Type, typename Allocator>
	void vector<Type, Allocator>::reserve(size_type size)
	{
		m_impl.reserve(size);
	}

	template<typename Type, typename Allocator>
	typename
	vector<Type, Allocator>::size_type vector<Type, Allocator>::capacity() const
	{
		return m_impl.end_of_storage - m_impl.begin;
	}

	template<typename Type, typename Allocator>
	void vector<Type, Allocator>::shrink_to_fit()
	{
		this_type(*this).swap(*this);
	}

	template<typename Type, typename Allocator>
	void vector<Type, Allocator>::clear()
	{
		if (!empty())
			this_type().swap(*this);
	}

	template<typename Type, typename Allocator>
	typename
	vector<Type, Allocator>::iterator vector<Type, Allocator>::insert(const_iterator cpos, const value_type& value)
	{
		return insert(cpos, (size_type)1, value);
	}

	template<typename Type, typename Allocator>
	typename
	vector<Type, Allocator>::iterator vector<Type, Allocator>::insert(const_iterator cpos, size_type n, const value_type& value)
	{
		simstd::pvt::_value_generator<Type> generator(value, n);
		return _insert(cpos, generator.begin(), generator.end(), simstd::pvt::iterator_category(generator.begin()));
//		iterator pos(begin());
//		simstd::advance(pos, simstd::distance(cbegin(), cpos));
//		if (m_impl.check_capacity(n)) {
//			iterator oldEnd(end());
//			size_type elems_between = oldEnd - pos;
//			if (elems_between < n) {
//				simstd::uninitialized_fill_n(oldEnd, n - elems_between, value);
//				simstd::uninitialized_copy(pos, oldEnd, oldEnd + n - elems_between);
//				simstd::fill_n(pos, elems_between, value);
//			} else {
//				simstd::uninitialized_copy(end() - n, end(), end());
//				simstd::copy_backward(pos, end() - n, end());
//				simstd::fill(pos, pos + n, value);
//			}
//			m_impl.end += n;
//			return pos;
//		}
//		impl_type newImpl(m_impl.get_new_capacity(n), m_impl.begin, &*pos);
//		iterator ret(newImpl.end);
//		simstd::uninitialized_fill_n(ret, n, value);
//		newImpl.end += n;
//		simstd::uninitialized_copy(pos, end(), iterator(newImpl.end));
//		newImpl.end += (end() - pos);
//		newImpl.swap(m_impl);
//		return ret;
	}

	template<typename Type, typename Allocator>
	template<typename InputIterator>
	typename
	vector<Type, Allocator>::iterator vector<Type, Allocator>::insert(const_iterator pos, InputIterator first, InputIterator last)
	{
		return _insert(pos, first, last, simstd::pvt::iterator_category(first));
	}

	template<typename Type, typename Allocator>
	typename
	vector<Type, Allocator>::iterator vector<Type, Allocator>::erase(const_iterator cfirst, const_iterator clast)
	{
		iterator first(begin());
		iterator last(begin());
		simstd::advance(first, simstd::distance(cbegin(), cfirst));
		simstd::advance(last, simstd::distance(cbegin(), clast));
		simstd::move(last, end(), first);
		size_type n = simstd::distance(cfirst, clast);
		m_impl.destroy(m_impl.end - n, m_impl.end);
		m_impl.end -= n;
		return iterator(first);
	}

	template<typename Type, typename Allocator>
	typename
	vector<Type, Allocator>::iterator vector<Type, Allocator>::erase(const_iterator pos)
	{
		return erase(pos, pos + 1);
	}

	template<typename Type, typename Allocator>
	void vector<Type, Allocator>::push_back(const value_type& value)
	{
		m_impl.adjust_capacity(1);
		simstd::uninitialized_fill_n(end(), 1, value);
		++m_impl.end;
	}

	template<typename Type, typename Allocator>
	void vector<Type, Allocator>::pop_back()
	{
		erase(cend() - 1, cend());
	}

	template<typename Type, typename Allocator>
	void vector<Type, Allocator>::resize(size_type count, const value_type& value)
	{
		if (size() < count) {
			_resize_increase(count, value);
		} else if (count < size()) {
			erase(cbegin() + count, cend());
		}
	}

	template<typename Type, typename Allocator>
	void vector<Type, Allocator>::resize(size_type count)
	{
		resize(count, value_type());
	}

	template<typename Type, typename Allocator>
	void vector<Type, Allocator>::swap(this_type& other)
	{
		m_impl.swap(other.m_impl);
	}

	template<typename Type, typename Allocator>
	void vector<Type, Allocator>::_resize_increase(size_type count, const value_type& value)
	{
		reserve(count);
		simstd::uninitialized_fill_n(end(), count - size(), value);
		m_impl.end += (count - size());
	}

	template<typename Type, typename Allocator>
	template<typename InputIterator>
	void vector<Type, Allocator>::_insert_back(InputIterator first, InputIterator last, simstd::input_iterator_tag)
	{
		simstd::copy(first, last, simstd::back_inserter(*this));
	}

	template<typename Type, typename Allocator>
	template<typename ForwardIterator>
	void vector<Type, Allocator>::_insert_back(ForwardIterator first, ForwardIterator last, simstd::forward_iterator_tag)
	{
		m_impl.adjust_capacity(simstd::distance(first, last));
		_insert_back(first, last, simstd::input_iterator_tag());
	}

	template<typename Type, typename Allocator>
	template<typename InputIterator>
	typename
	vector<Type, Allocator>::iterator vector<Type, Allocator>::_insert(const_iterator pos, InputIterator first, InputIterator last, simstd::input_iterator_tag)
	{
		difference_type distance = simstd::distance(cbegin(), pos);
		simstd::copy(first, last, simstd::inserter(*this, pos));
		return begin() + distance;
	}

	template<typename Type, typename Allocator>
	template<typename ForwardIterator>
	typename
	vector<Type, Allocator>::iterator vector<Type, Allocator>::_insert(const_iterator cpos, ForwardIterator first, ForwardIterator last, simstd::forward_iterator_tag)
	{
		iterator pos(begin());
		simstd::advance(pos, simstd::distance(cbegin(), cpos));
		difference_type distance = simstd::distance(begin(), pos);
		difference_type n = simstd::distance(first, last);
		iterator oldEnd(end());
		if (m_impl.check_capacity(n)) {
			difference_type elems_between = oldEnd - pos;
			if (elems_between < n) {
				ForwardIterator mid(first);
				simstd::advance(mid, elems_between);
				simstd::uninitialized_copy(mid, last, oldEnd);
				simstd::uninitialized_copy(pos, oldEnd, oldEnd + n - elems_between);
				simstd::copy(first, mid, pos);
			} else {
				simstd::uninitialized_copy(oldEnd - n, oldEnd, oldEnd);
				simstd::copy_backward(pos, oldEnd - n, oldEnd);
				simstd::copy(first, last, pos);
			}
			m_impl.end += n;
			return pos;
		}
		impl_type newImpl(m_impl.get_new_capacity(n), m_impl.begin, &*pos);
		iterator ret(newImpl.end);
		simstd::uninitialized_copy(first, last, ret);
		newImpl.end += n;
		simstd::uninitialized_copy(pos, oldEnd, ret + n);
		newImpl.end += (oldEnd - pos);
		newImpl.swap(m_impl);
		return ret;
	}

}

#endif
