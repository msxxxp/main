#ifndef BASIS_LIST_INTERFACE_HPP_
#define BASIS_LIST_INTERFACE_HPP_

#include <basis/std/list>

namespace simstd {

	template<typename Type, typename Allocator = simstd::allocator<Type> >
	class list: private pvt::List_base<Type, Allocator> {
		typedef list this_type;
		typedef pvt::List_base<Type, Allocator>        base_type;
		typedef typename base_type::Type_alloc_type    Type_alloc_type;
		typedef typename base_type::Node_alloc_type    Node_alloc_type;

	public:
		typedef Allocator allocator_type;
		typedef size_t    size_type;

		typedef allocator_traits<allocator_type>       alloc_traits;
		typedef typename alloc_traits::value_type      value_type;
		typedef typename alloc_traits::pointer         pointer;
		typedef typename alloc_traits::const_pointer   const_pointer;
		typedef typename alloc_traits::reference       reference;
		typedef typename alloc_traits::const_reference const_reference;
		typedef typename alloc_traits::difference_type difference_type;

		typedef simstd::pvt::List_iterator<pointer>       iterator;
		typedef simstd::pvt::List_const_iterator<pointer> const_iterator;
		typedef simstd::reverse_iterator<iterator>        reverse_iterator;
		typedef simstd::reverse_iterator<const_iterator>  const_reverse_iterator;

	public:
		~list();

		list();
		explicit list(const allocator_type& allocator);
		list(size_type count, const value_type& value, const allocator_type& allocator = allocator_type());
		explicit list(size_type count, const allocator_type& allocator = allocator_type());

		template<class InputIt>
		list(InputIt first, InputIt last, const allocator_type& allocator = allocator_type());
		list(const this_type & other);
		list(const this_type & other, const allocator_type & allocator);
		list(this_type && other);
		list(this_type && other, const allocator_type & allocator);

		this_type& operator =(const this_type& other);
		this_type& operator =(this_type&& other);

		template<typename InputIterator>
		void assign(InputIterator first, InputIterator last);
		void assign(size_type n, const value_type& value);

		allocator_type get_allocator() const;

		reference         front();
		const_reference   front() const;
		reference         back();
		const_reference   back() const;

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

		void      clear();

		template<typename InputIterator>
		iterator insert(const_iterator pos, InputIterator first, InputIterator last);
		iterator insert(const_iterator pos, const value_type& value);
		iterator insert(const_iterator pos, size_type n, const value_type& value);
		iterator insert(const_iterator pos, value_type&& value);

		template<typename... Args>
		iterator emplace(const_iterator pos, Args&&... args);

		iterator erase(const_iterator pos);
		iterator erase(const_iterator first, const_iterator last);

		void     push_back(const value_type& value);
		void     push_back(value_type&& value);

		template<typename... Args>
		void     emplace_back(Args&&... args);

		void     pop_back();

		void     push_front(const value_type& value);
		void     push_front(value_type&& value);

		template<typename... Args>
		void     emplace_front(Args&&... args);

		void     pop_front();

		void     resize(size_type count);
		void     resize(size_type count, const value_type& value);

		void     swap(this_type& other);

		void     merge(this_type& other);
		void     merge(this_type&& other);
		template<class Compare>
		void     merge(this_type& other, Compare comp);
		template<class Compare>
		void     merge(this_type&& other, Compare comp);

		void     splice(const_iterator pos, this_type& other);
		void     splice(const_iterator pos, this_type&& other);
		void     splice(const_iterator pos, this_type& other, const_iterator it);
		void     splice(const_iterator pos, this_type&& other, const_iterator it);
		void     splice(const_iterator pos, this_type& other, const_iterator first, const_iterator last);
		void     splice(const_iterator pos, this_type&& other, const_iterator first, const_iterator last);

		void     remove(const value_type& value);
		template<class UnaryPredicate>
		void     remove_if(UnaryPredicate p);

		void     reverse();

		void     unique();
		template<class BinaryPredicate>
		void     unique(BinaryPredicate p);

		void     sort();
		template<class Compare>
		void     sort(Compare comp);

	protected:
		typedef simstd::pvt::vector_base<value_type, allocator_type> impl_type;
		impl_type m_impl;

	private:
		template<typename... Args>
		iterator _emplace(const_iterator cpos, Args&&... args);

		iterator _erase(const_iterator first, const_iterator last);

		template<typename... Args>
		void _emplace_back(Args&&... args);

		void _resize_increase(size_type count, const value_type& value);

		template<typename InputIterator>
		void _insert_back(InputIterator first, InputIterator last, simstd::input_iterator_tag);

		template<typename ForwardIterator>
		void _insert_back(ForwardIterator first, ForwardIterator last, simstd::forward_iterator_tag);

		iterator _insert(const_iterator pos, size_type n, const value_type& value);

		template<typename InputIterator>
		iterator _insert(const_iterator pos, InputIterator first, InputIterator last, simstd::input_iterator_tag);

		template<typename ForwardIterator>
		iterator _insert(const_iterator pos, ForwardIterator first, ForwardIterator last, simstd::forward_iterator_tag);
	};

	///=============================================================================================
	template<typename Type, typename Allocator>
	bool operator ==(const list<Type, Allocator>& lhs, const list<Type, Allocator>& rhs)
	{
		return simstd::equal(lhs.begin(), lhs.end(), rhs.begin());
	}

	template<typename Type, typename Allocator>
	bool operator !=(const list<Type, Allocator>& lhs, const list<Type, Allocator>& rhs)
	{
		return simstd::rel_ops::operator !=(lhs, rhs);
	}

	template<typename Type, typename Allocator>
	bool operator <(const list<Type, Allocator>& lhs, const list<Type, Allocator>& rhs)
	{
		return simstd::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
	}

	template<typename Type, typename Allocator>
	bool operator >(const list<Type, Allocator>& lhs, const list<Type, Allocator>& rhs)
	{
		return simstd::rel_ops::operator >(lhs, rhs);
	}

	template<typename Type, typename Allocator>
	bool operator <=(const list<Type, Allocator>& lhs, const list<Type, Allocator>& rhs)
	{
		return simstd::rel_ops::operator <=(lhs, rhs);
	}

	template<typename Type, typename Allocator>
	bool operator >=(const list<Type, Allocator>& lhs, const list<Type, Allocator>& rhs)
	{
		return simstd::rel_ops::operator >=(lhs, rhs);
	}

	template<typename Type, typename Allocator>
	void swap(list<Type, Allocator>& lhs, list<Type, Allocator>& rhs)
	{
		lhs.swap(rhs);
	}

	///=============================================================================================
	template<typename Type, typename Allocator>
	list<Type, Allocator>::~list()
	{
	}

	template<typename Type, typename Allocator>
	list<Type, Allocator>::list():
		list(Allocator())
	{
	}

	template<typename Type, typename Allocator>
	list<Type, Allocator>::list(const allocator_type & allocator):
		base_type(Node_alloc_type(allocator))
	{
	}

	template<typename Type, typename Allocator>
	list<Type, Allocator>::list(size_type count, const value_type& value, const allocator_type& allocator):
		base_type(Node_alloc_type(allocator))
	{
	}

	template<typename Type, typename Allocator>
	list<Type, Allocator>::list(size_type count, const allocator_type& allocator):
		base_type(Node_alloc_type(allocator))
	{
	}

	template<typename Type, typename Allocator>
	template<class InputIt>
	list<Type, Allocator>::list(InputIt first, InputIt last, const allocator_type& allocator):
		base_type(Node_alloc_type(allocator))
	{
	}

	template<typename Type, typename Allocator>
	list<Type, Allocator>::list(const this_type & other):
		base_type(other._M_get_Node_allocator())
	{
	}

	template<typename Type, typename Allocator>
	list<Type, Allocator>::list(const this_type & other, const allocator_type & allocator):
		base_type(Node_alloc_type(allocator))
	{
	}

	template<typename Type, typename Allocator>
	list<Type, Allocator>::list(this_type && other):
		base_type(other._M_get_Node_allocator())
	{
	}

	template<typename Type, typename Allocator>
	list<Type, Allocator>::list(this_type && other, const allocator_type & allocator):
		base_type(Node_alloc_type(allocator))
	{
	}









	template<typename Type, typename Allocator>
	vector<Type, Allocator>::vector(size_type n, const allocator_type& allocator):
		m_impl(n, allocator)
	{
		m_impl.end = simstd::pvt::_construct_default(m_impl.allocator, m_impl.begin, m_impl.begin + n);
	}

	template<typename Type, typename Allocator>
	vector<Type, Allocator>::vector(size_type n, const value_type& value, const allocator_type& allocator) :
		m_impl(n, allocator)
	{
		m_impl.end = simstd::uninitialized_fill_n(m_impl.begin, n, value);
	}

	template<typename Type, typename Allocator>
	template<typename InputIterator>
	vector<Type, Allocator>::vector(InputIterator first, InputIterator last, const allocator_type& allocator) :
		m_impl(allocator)
	{
		_insert_back(first, last, simstd::pvt::iterator_category(first));
	}

	template<typename Type, typename Allocator>
	vector<Type, Allocator>::vector(const this_type& other) :
		m_impl(other.size(), other.m_impl.allocator)
	{
		m_impl.end = simstd::uninitialized_copy(other.m_impl.begin, other.m_impl.end, m_impl.end);
	}

	template<typename Type, typename Allocator>
	vector<Type, Allocator>::vector(const this_type& other, const allocator_type& allocator) :
		m_impl(other.size(), allocator)
	{
		m_impl.end = simstd::uninitialized_copy(other.m_impl.begin, other.m_impl.end, m_impl.end);
	}

	template<typename Type, typename Allocator>
	vector<Type, Allocator>::vector(this_type&& other) :
		m_impl(simstd::move(other.m_impl))
	{
	}

	template<typename Type, typename Allocator>
	vector<Type, Allocator>::vector(this_type&& other, const allocator_type& allocator) :
		m_impl(simstd::move(other.m_impl), allocator)
	{
	}

	template<typename Type, typename Allocator>
	typename
	vector<Type, Allocator>::this_type& vector<Type, Allocator>::operator =(const this_type& other)
	{
		FuncTrace();
		this_type(other).swap(*this);
		return *this;
	}

	template<typename Type, typename Allocator>
	void vector<Type, Allocator>::assign(size_type n, const value_type& value)
	{
		FuncTrace();
		this_type(n, value).swap(*this);
	}

	template<typename Type, typename Allocator>
	template<typename InputIterator>
	void vector<Type, Allocator>::assign(InputIterator first, InputIterator last)
	{
		FuncTrace();
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
		FuncTrace();
		this_type(*this).swap(*this);
	}

	template<typename Type, typename Allocator>
	void vector<Type, Allocator>::clear()
	{
		FuncTrace();
		if (!empty())
			m_impl.clear();
	}

	template<typename Type, typename Allocator>
	template<typename InputIterator>
	typename
	vector<Type, Allocator>::iterator vector<Type, Allocator>::insert(const_iterator pos, InputIterator first, InputIterator last)
	{
		FuncTrace();
		return _insert(pos, first, last, simstd::pvt::iterator_category(first));
	}

	template<typename Type, typename Allocator>
	typename
	vector<Type, Allocator>::iterator vector<Type, Allocator>::insert(const_iterator cpos, const value_type& value)
	{
		FuncTrace();
		return _insert(cpos, static_cast<size_type>(1), value);
	}

	template<typename Type, typename Allocator>
	typename
	vector<Type, Allocator>::iterator vector<Type, Allocator>::insert(const_iterator cpos, size_type n, const value_type& value)
	{
		FuncTrace();
		return _insert(cpos, n, value);
	}

	template<typename Type, typename Allocator>
	typename
	vector<Type, Allocator>::iterator vector<Type, Allocator>::insert(const_iterator pos, value_type&& value)
	{
		FuncTrace();
		return emplace(pos, simstd::move(value));
	}

	template<typename Type, typename Allocator>
	template<typename... Args>
	typename
	vector<Type, Allocator>::iterator vector<Type, Allocator>::emplace(const_iterator cpos, Args&&... args)
	{
		FuncTrace();
		return _emplace(cpos, simstd::forward<Args>(args)...);
	}

	template<typename Type, typename Allocator>
	typename
	vector<Type, Allocator>::iterator vector<Type, Allocator>::erase(const_iterator cpos)
	{
		FuncTrace();
		return _erase(cpos, cpos + 1);
	}

	template<typename Type, typename Allocator>
	typename
	vector<Type, Allocator>::iterator vector<Type, Allocator>::erase(const_iterator cfirst, const_iterator clast)
	{
		FuncTrace();
		return _erase(cfirst, clast);
	}

	template<typename Type, typename Allocator>
	void vector<Type, Allocator>::push_back(const value_type& value)
	{
		FuncTrace();
		_emplace_back(value);
	}

	template<typename Type, typename Allocator>
	void vector<Type, Allocator>::push_back(value_type&& value)
	{
		FuncTrace();
		_emplace_back(simstd::move(value));
	}

	template<typename Type, typename Allocator>
	template<typename... Args>
	void vector<Type, Allocator>::emplace_back(Args&&... args)
	{
		FuncTrace();
		_emplace_back(simstd::forward<Args>(args)...);
	}

	template<typename Type, typename Allocator>
	void vector<Type, Allocator>::pop_back()
	{
		FuncTrace();
		_erase(cend() - 1, cend());
	}

	template<typename Type, typename Allocator>
	void vector<Type, Allocator>::resize(size_type count, const value_type& value)
	{
		FuncTrace();
		if (size() < count) {
			_resize_increase(count, value);
		} else if (count < size()) {
			_erase(cbegin() + count, cend());
		}
	}

	template<typename Type, typename Allocator>
	void vector<Type, Allocator>::resize(size_type count)
	{
		FuncTrace();
		resize(count, value_type());
	}

	template<typename Type, typename Allocator>
	void vector<Type, Allocator>::swap(this_type& other)
	{
		FuncTrace();
		m_impl.swap(other.m_impl);
	}

	template<typename Type, typename Allocator>
	template<typename... Args>
	typename
	vector<Type, Allocator>::iterator vector<Type, Allocator>::_emplace(const_iterator cpos, Args&&... args)
	{
		auto distance = simstd::distance(cbegin(), cpos);
		iterator pos = simstd::next(begin(), distance);
		if (m_impl.check_capacity_if_size_grows(1)) {
			FuncTrace();
			if (cpos == cend()) {
				m_impl.construct(m_impl.end, simstd::forward<Args>(args)...);
			} else {
				simstd::uninitialized_copy(simstd::make_move_iterator(m_impl.end - 1), simstd::make_move_iterator(m_impl.end), m_impl.end);
				simstd::move_backward(pos, end() - 1, end());
				*pos = value_type(simstd::forward<Args>(args)...);
			}
			++m_impl.end;
		} else {
			FuncTrace();
			impl_type newImpl(m_impl.get_new_capacity(1), m_impl.allocator);
			newImpl.end = simstd::uninitialized_copy(simstd::make_move_iterator(m_impl.begin), simstd::make_move_iterator(&*pos), newImpl.end);

			newImpl.construct(newImpl.end, simstd::forward<Args>(args)...);
			++newImpl.end;

			newImpl.end = simstd::uninitialized_copy(simstd::make_move_iterator(&*pos), simstd::make_move_iterator(m_impl.end), newImpl.end);
			newImpl.swap(m_impl);
		}
		return iterator(m_impl.begin + distance);
	}

	template<typename Type, typename Allocator>
	typename
	vector<Type, Allocator>::iterator vector<Type, Allocator>::_erase(const_iterator cfirst, const_iterator clast)
	{
		auto first = simstd::next(begin(), simstd::distance(cbegin(), cfirst));
		auto last = simstd::next(begin(), simstd::distance(cbegin(), clast));
		auto newEnd = simstd::move(last, end(), first);
		m_impl.destroy(&*newEnd, m_impl.end);
		m_impl.end = &*newEnd;
		return iterator(first);
	}

	template<typename Type, typename Allocator>
	template<typename... Args>
	void vector<Type, Allocator>::_emplace_back(Args&&... args)
	{
		m_impl.adjust_capacity(1);
		m_impl.construct(m_impl.end++, simstd::forward<Args>(args)...);
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
		FuncTrace();
		simstd::copy(first, last, simstd::back_inserter(*this));
	}

	template<typename Type, typename Allocator>
	template<typename ForwardIterator>
	void vector<Type, Allocator>::_insert_back(ForwardIterator first, ForwardIterator last, simstd::forward_iterator_tag)
	{
		FuncTrace();
		m_impl.adjust_capacity(simstd::distance(first, last));
		m_impl.end = simstd::uninitialized_copy(first, last, m_impl.end);
	}

	template<typename Type, typename Allocator>
	typename
	vector<Type, Allocator>::iterator vector<Type, Allocator>::_insert(const_iterator cpos, size_type n, const value_type& value)
	{
		FuncTrace();
		simstd::pvt::_value_generator<Type> generator(n, value);
		return _insert(cpos, generator.begin(), generator.end(), simstd::pvt::iterator_category(generator.begin()));
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
		difference_type distance = simstd::distance(cbegin(), cpos);
		iterator pos = simstd::next(begin(), distance);
		difference_type n = simstd::distance(first, last);
		if (m_impl.check_capacity_if_size_grows(n)) {
			difference_type elemsToMove = end() - pos;
			iterator oldEnd(end());
			if (elemsToMove < n) {
				auto mid = simstd::next(first, elemsToMove);
				m_impl.end = simstd::uninitialized_copy(mid, last, m_impl.end);
				m_impl.end = simstd::uninitialized_copy(simstd::make_move_iterator(pos), simstd::make_move_iterator(oldEnd), m_impl.end);
				simstd::copy(first, mid, pos);
			} else {
				m_impl.end = simstd::uninitialized_copy_n(simstd::make_move_iterator(oldEnd - n), n, m_impl.end);
				simstd::move_backward(pos, oldEnd - n, oldEnd);
				simstd::copy(first, last, pos);
			}
			return pos;
		}
		impl_type newImpl(m_impl.get_new_capacity(n), m_impl.allocator);
		newImpl.end = simstd::uninitialized_copy(simstd::make_move_iterator(m_impl.begin), simstd::make_move_iterator(&*pos), newImpl.end);
		iterator ret(newImpl.end);
		newImpl.end = simstd::uninitialized_copy(first, last, newImpl.end);
		newImpl.end = simstd::uninitialized_copy(simstd::make_move_iterator(&*pos), simstd::make_move_iterator(m_impl.end), newImpl.end);
		newImpl.swap(m_impl);
		return ret;
	}

}

#endif
