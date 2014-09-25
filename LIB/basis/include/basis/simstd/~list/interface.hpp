#ifndef BASIS_LIST_INTERFACE_HPP_
#define BASIS_LIST_INTERFACE_HPP_

#include <basis/simstd/list>
#include <basis/sys/logger.hpp>

namespace simstd {

	template<typename Type, typename Allocator = simstd::allocator<Type> >
	class list: private pvt::List_base<Type, Allocator> {
		typedef list                                   this_type;
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

		typedef simstd::pvt::List_iterator<value_type>       iterator;
		typedef simstd::pvt::List_const_iterator<value_type> const_iterator;
		typedef simstd::reverse_iterator<iterator>           reverse_iterator;
		typedef simstd::reverse_iterator<const_iterator>     const_reverse_iterator;

	public:
		~list();

		list();
		explicit list(const allocator_type& allocator);
		list(size_type count, const value_type& value, const allocator_type& allocator = allocator_type());
		explicit list(size_type count, const allocator_type& allocator = allocator_type());

		template<class InputIterator>
		list(InputIterator first, InputIterator last, const allocator_type& allocator = allocator_type());
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
		void     remove_if(UnaryPredicate predicatep);

		void     reverse();

		void     unique();
		template<class BinaryPredicate>
		void     unique(BinaryPredicate predicate);

		void     sort();
		template<class Compare>
		void     sort(Compare comp);

	private:
		void _transfer(const_iterator position, const_iterator first, const_iterator last);

		void _default_append(size_type count);

		template<class InputIterator>
		void _initialize(InputIterator first, InputIterator last);
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
		LogTraceObj();
		clear();
	}

	template<typename Type, typename Allocator>
	list<Type, Allocator>::list():
		list(Allocator())
	{
		LogTraceObj();
	}

	template<typename Type, typename Allocator>
	list<Type, Allocator>::list(const allocator_type & allocator):
		base_type(Node_alloc_type(allocator))
	{
		LogTraceObj();
	}

	template<typename Type, typename Allocator>
	list<Type, Allocator>::list(size_type count, const value_type& value, const allocator_type& allocator):
		base_type(Node_alloc_type(allocator))
	{
		LogTraceObj();
		for (; count; --count)
			emplace_back(value);
	}

	template<typename Type, typename Allocator>
	list<Type, Allocator>::list(size_type count, const allocator_type& allocator):
		base_type(Node_alloc_type(allocator))
	{
		LogTraceObj();
		_default_append(count);
	}

	template<typename Type, typename Allocator>
	template<class InputIterator>
	list<Type, Allocator>::list(InputIterator first, InputIterator last, const allocator_type& allocator):
		base_type(Node_alloc_type(allocator))
	{
		LogTraceObj();
		_initialize(first, last);
	}

	template<typename Type, typename Allocator>
	list<Type, Allocator>::list(const this_type & other):
		base_type(other.get_node_allocator())
	{
		LogTraceObj();
		_initialize(other.begin(), other.end());
	}

	template<typename Type, typename Allocator>
	list<Type, Allocator>::list(const this_type & other, const allocator_type & allocator):
		base_type(Node_alloc_type(allocator))
	{
		LogTraceObj();
		_initialize(other.begin(), other.end());
	}

	template<typename Type, typename Allocator>
	list<Type, Allocator>::list(this_type && other):
		base_type(simstd::move(other))
	{
		LogTraceObj();
	}

	template<typename Type, typename Allocator>
	list<Type, Allocator>::list(this_type && other, const allocator_type & allocator):
		base_type(Node_alloc_type(allocator))
	{
		LogTraceObj();
	}

	template<typename Type, typename Allocator>
	typename
	list<Type, Allocator>::this_type& list<Type, Allocator>::operator =(const this_type& other)
	{
		LogTraceObj();
		this_type(other).swap(*this);
		return *this;
	}

	template<typename Type, typename Allocator>
	typename
	list<Type, Allocator>::this_type& list<Type, Allocator>::operator =(this_type && other)
	{
		LogTraceObj();
		this_type(simstd::move(other)).swap(*this);
		return *this;
	}

	template<typename Type, typename Allocator>
	template<typename InputIterator>
	void list<Type, Allocator>::assign(InputIterator first, InputIterator last)
	{
		LogTraceObj();
		this_type(first, last).swap(*this);
	}

	template<typename Type, typename Allocator>
	void list<Type, Allocator>::assign(size_type n, const value_type& value)
	{
		LogTraceObj();
		this_type(n, value).swap(*this);
	}

	template<typename Type, typename Allocator>
	typename
	list<Type, Allocator>::allocator_type list<Type, Allocator>::get_allocator() const
	{
		LogTraceObj();
		return base_type::get_allocator();
	}

	template<typename Type, typename Allocator>
	typename
	list<Type, Allocator>::reference list<Type, Allocator>::front()
	{
		return *begin();
	}

	template<typename Type, typename Allocator>
	typename
	list<Type, Allocator>::const_reference list<Type, Allocator>::front() const
	{
		return *begin();
	}

	template<typename Type, typename Allocator>
	typename
	list<Type, Allocator>::reference list<Type, Allocator>::back()
	{
		iterator tmp = end();
		return *(--tmp);
	}

	template<typename Type, typename Allocator>
	typename
	list<Type, Allocator>::const_reference list<Type, Allocator>::back() const
	{
		const_iterator tmp = end();
		return *(--tmp);
	}

	template<typename Type, typename Allocator>
	typename
	list<Type, Allocator>::iterator list<Type, Allocator>::begin()
	{
		return iterator(base_type::m_impl.m_end.m_next);
	}

	template<typename Type, typename Allocator>
	typename
	list<Type, Allocator>::const_iterator list<Type, Allocator>::begin() const
	{
		return cbegin();
	}

	template<typename Type, typename Allocator>
	typename
	list<Type, Allocator>::const_iterator list<Type, Allocator>::cbegin() const
	{
		return const_iterator(base_type::m_impl.m_end.m_next);
	}

	template<typename Type, typename Allocator>
	typename
	list<Type, Allocator>::iterator list<Type, Allocator>::end()
	{
		return iterator(&base_type::m_impl.m_end);
	}

	template<typename Type, typename Allocator>
	typename
	list<Type, Allocator>::const_iterator list<Type, Allocator>::end() const
	{
		return cend();
	}

	template<typename Type, typename Allocator>
	typename
	list<Type, Allocator>::const_iterator list<Type, Allocator>::cend() const
	{
		return const_iterator(&base_type::m_impl.m_end);
	}

	template<typename Type, typename Allocator>
	typename
	list<Type, Allocator>::reverse_iterator list<Type, Allocator>::rbegin()
	{
		return reverse_iterator(end());
	}

	template<typename Type, typename Allocator>
	typename
	list<Type, Allocator>::const_reverse_iterator list<Type, Allocator>::rbegin() const
	{
		return crbegin();
	}

	template<typename Type, typename Allocator>
	typename
	list<Type, Allocator>::const_reverse_iterator list<Type, Allocator>::crbegin() const
	{
		return const_reverse_iterator(end());
	}

	template<typename Type, typename Allocator>
	typename
	list<Type, Allocator>::reverse_iterator list<Type, Allocator>::rend()
	{
		return reverse_iterator(begin());
	}

	template<typename Type, typename Allocator>
	typename
	list<Type, Allocator>::const_reverse_iterator list<Type, Allocator>::rend() const
	{
		return crend();
	}

	template<typename Type, typename Allocator>
	typename
	list<Type, Allocator>::const_reverse_iterator list<Type, Allocator>::crend() const
	{
		return const_reverse_iterator(begin());
	}

	template<typename Type, typename Allocator>
	bool list<Type, Allocator>::empty() const
	{
		return base_type::m_impl.m_end.m_next == &base_type::m_impl.m_end;
	}

	template<typename Type, typename Allocator>
	typename
	list<Type, Allocator>::size_type list<Type, Allocator>::size() const
	{
		return simstd::distance(begin(), end());
	}

	template<typename Type, typename Allocator>
	typename
	list<Type, Allocator>::size_type list<Type, Allocator>::max_size() const
	{
		return base_type::get_node_allocator().max_size();
	}

	template<typename Type, typename Allocator>
	void list<Type, Allocator>::clear()
	{
		while (!empty())
			pop_back();
	}

	template<typename Type, typename Allocator>
	template<typename InputIterator>
	typename
	list<Type, Allocator>::iterator list<Type, Allocator>::insert(const_iterator pos, InputIterator first, InputIterator last)
	{
		LogTraceObj();
		this_type tmp(first, last, get_allocator());
		splice(pos, tmp);
	}

	template<typename Type, typename Allocator>
	typename
	list<Type, Allocator>::iterator list<Type, Allocator>::insert(const_iterator pos, const value_type& value)
	{
		LogTraceObj();
		return emplace(pos, value);
	}

	template<typename Type, typename Allocator>
	typename
	list<Type, Allocator>::iterator list<Type, Allocator>::insert(const_iterator pos, size_type n, const value_type& value)
	{
		LogTraceObj();
		this_type tmp(n, value, get_allocator());
		splice(pos, tmp);
	}

	template<typename Type, typename Allocator>
	typename
	list<Type, Allocator>::iterator list<Type, Allocator>::insert(const_iterator pos, value_type&& value)
	{
		LogTraceObj();
		return emplace(pos, simstd::move(value));
	}

	template<typename Type, typename Allocator>
	template<typename... Args>
	typename
	list<Type, Allocator>::iterator list<Type, Allocator>::emplace(const_iterator pos, Args&&... args)
	{
		LogTraceObj();
		auto tmp = base_type::new_node(simstd::forward<Args>(args)...);
		tmp->hook(pos.m_node);
		return iterator(tmp);
	}

	template<typename Type, typename Allocator>
	typename
	list<Type, Allocator>::iterator list<Type, Allocator>::erase(const_iterator pos)
	{
		LogTraceObj();
		iterator ret(pos.m_node->m_next);

		iterator it(pos.iterator_cast());
		it.m_node->unhook();
		base_type::delete_node(it.m_node);

		return ret;
	}

	template<typename Type, typename Allocator>
	typename
	list<Type, Allocator>::iterator list<Type, Allocator>::erase(const_iterator first, const_iterator last)
	{
		LogTraceObj();
		while (first != last)
			first = erase(first);
		return last.iterator_cast();
	}

	template<typename Type, typename Allocator>
	void list<Type, Allocator>::push_back(const value_type& value)
	{
		LogTraceObj();
		emplace(cend(), value);
	}

	template<typename Type, typename Allocator>
	void list<Type, Allocator>::push_back(value_type&& value)
	{
		LogTraceObj();
		emplace(cend(), simstd::move(value));
	}

	template<typename Type, typename Allocator>
	template<typename... Args>
	void list<Type, Allocator>::emplace_back(Args&&... args)
	{
		LogTraceObj();
		emplace(cend(), simstd::forward<Args>(args)...);
	}

	template<typename Type, typename Allocator>
	void list<Type, Allocator>::pop_back()
	{
		LogTraceObj();
		const_iterator pos = cend();
		erase(--pos);
	}

	template<typename Type, typename Allocator>
	void list<Type, Allocator>::push_front(const value_type& value)
	{
		LogTraceObj();
		emplace(cbegin(), value);
	}

	template<typename Type, typename Allocator>
	void list<Type, Allocator>::push_front(value_type&& value)
	{
		LogTraceObj();
		emplace(cbegin(), simstd::move(value));
	}

	template<typename Type, typename Allocator>
	template<typename... Args>
	void list<Type, Allocator>::emplace_front(Args&&... args)
	{
		LogTraceObj();
		emplace(cbegin(), simstd::forward<Args>(args)...);
	}

	template<typename Type, typename Allocator>
	void list<Type, Allocator>::pop_front()
	{
		LogTraceObj();
		erase(cbegin());
	}

	template<typename Type, typename Allocator>
	void list<Type, Allocator>::resize(size_type count)
	{
		auto it = cbegin();
		size_type len = 0;

		for (; it != end() && len < count; ++it, ++len)
			;

		if (len == count)
			erase(it, cend());
		else
			_default_append(count - len);
	}

	template<typename Type, typename Allocator>
	void list<Type, Allocator>::resize(size_type count, const value_type& value)
	{
		auto it = cbegin();
		size_type len = 0;

		for (; it != end() && len < count; ++it, ++len)
			;

		if (len == count)
			erase(it, cend());
		else
			insert(cend(), count - len, value);
	}

	template<typename Type, typename Allocator>
	void list<Type, Allocator>::swap(this_type& other)
	{
		base_type::m_impl.m_end.swap(other.m_impl.m_end);
	}

	template<typename Type, typename Allocator>
	void list<Type, Allocator>::merge(this_type& other)
	{
		merge(simstd::move(other));
	}

	template<typename Type, typename Allocator>
	void list<Type, Allocator>::merge(this_type&& other)
	{
		if (this != &other) {
			auto first1 = begin();
			auto last1 = end();
			auto first2 = other.begin();
			auto last2 = other.end();

			while (first1 != last1 && first2 != last2) {
				if (*first2 < *first1) {
					auto next = first2;
					_transfer(first1, first2, ++next);
					first2 = next;
				} else
					++first1;
			}

			if (first2 != last2)
				_transfer(last1, first2, last2);
		}
	}

	template<typename Type, typename Allocator>
	template<class Compare>
	void list<Type, Allocator>::merge(this_type& other, Compare comp)
	{
		merge(simstd::move(other), comp);
	}

	template<typename Type, typename Allocator>
	template<class Compare>
	void list<Type, Allocator>::merge(this_type&& other, Compare comp)
	{
		if (this != &other) {
			auto first1 = begin();
			auto last1 = end();
			auto first2 = other.begin();
			auto last2 = other.end();
			while (first1 != last1 && first2 != last2) {
				if (comp(*first2, *first1)) {
					auto next = first2;
					_transfer(first1, first2, ++next);
					first2 = next;
				} else
					++first1;
			}
			if (first2 != last2)
				_transfer(last1, first2, last2);
		}
	}

	template<typename Type, typename Allocator>
	void list<Type, Allocator>::splice(const_iterator pos, this_type& other)
	{
		splice(pos, simstd::move(other));
	}

	template<typename Type, typename Allocator>
	void list<Type, Allocator>::splice(const_iterator pos, this_type&& other)
	{
		_transfer(pos.iterator_cast(), other.begin(), other.end());
	}

	template<typename Type, typename Allocator>
	void list<Type, Allocator>::splice(const_iterator pos, this_type& other, const_iterator it)
	{
		splice(pos, simstd::move(other), it);
	}

	template<typename Type, typename Allocator>
	void list<Type, Allocator>::splice(const_iterator pos, this_type&& other, const_iterator it)
	{
		auto j = it.iterator_cast();
		++j;

		if (pos == it || pos == j)
			return;

		_transfer(pos.iterator_cast(), it.iterator_cast(), j);
	}

	template<typename Type, typename Allocator>
	void list<Type, Allocator>::splice(const_iterator pos, this_type& other, const_iterator first, const_iterator last)
	{
		splice(pos, simstd::move(other), first, last);
	}

	template<typename Type, typename Allocator>
	void list<Type, Allocator>::splice(const_iterator pos, this_type&& other, const_iterator first, const_iterator last)
	{
		if (first != last)
			_transfer(pos.iterator_cast(), first.iterator_cast(), last.iterator_cast());
	}

	template<typename Type, typename Allocator>
	void list<Type, Allocator>::remove(const value_type& value)
	{
		auto first = cbegin();
		auto last = cend();
		auto postponed = last;

		while (first != last) {
			auto next = first;
			++next;
			if (*first == value) {
				if (simstd::addressof(*first) == simstd::addressof(value))
					postponed = first;
				else
					erase(first);
			}
			first = next;
		}

		if (postponed != last)
			erase(postponed);
	}

	template<typename Type, typename Allocator>
	template<class UnaryPredicate>
	void list<Type, Allocator>::remove_if(UnaryPredicate predicate)
	{
		auto first = cbegin();
		auto last = cend();
		while (first != last)
		{
			if (predicate(*first))
				first = erase(first);
			else
				++first;
		}
	}

	template<typename Type, typename Allocator>
	void list<Type, Allocator>::reverse()
	{
		LogTraceObj();
		base_type::m_impl.m_end.reverse();
	}

	template<typename Type, typename Allocator>
	void list<Type, Allocator>::unique()
	{
		auto first = cbegin();
		auto last = cend();

		if (first == last)
			return;

		auto next = first;
		while (++next != last)
		{
			if (*first == *next)
				erase(next);
			else
				first = next;
			next = first;
		}
	}

	template<typename Type, typename Allocator>
	template<class BinaryPredicate>
	void list<Type, Allocator>::unique(BinaryPredicate predicate)
	{
		auto first = cbegin();
		auto last = cend();

		if (first == last)
			return;

		auto next = first;
		while (++next != last)
		{
			if (predicate(*first, *next))
				erase(next);
			else
				first = next;
			next = first;
		}
	}

	template<typename Type, typename Allocator>
	void list<Type, Allocator>::sort()
	{
		if (size() > 1) {
			list __carry;
			list __tmp[64];
			list * __fill = &__tmp[0];
			list * __counter;

			do {
				__carry.splice(__carry.begin(), *this, begin());

				for (__counter = &__tmp[0]; __counter != __fill && !__counter->empty(); ++__counter) {
					__counter->merge(__carry);
					__carry.swap(*__counter);
				}
				__carry.swap(*__counter);
				if (__counter == __fill)
					++__fill;
			} while (!empty());

			for (__counter = &__tmp[1]; __counter != __fill; ++__counter)
				__counter->merge(*(__counter - 1));
			swap(*(__fill - 1));
		}
	}

	template<typename Type, typename Allocator>
	template<class Compare>
	void list<Type, Allocator>::sort(Compare comp)
	{
		if (size() > 1) {
			list __carry;
			list __tmp[64];
			list * __fill = &__tmp[0];
			list * __counter;

			do {
				__carry.splice(__carry.begin(), *this, begin());

				for (__counter = &__tmp[0]; __counter != __fill && !__counter->empty(); ++__counter) {
					__counter->merge(__carry, comp);
					__carry.swap(*__counter);
				}
				__carry.swap(*__counter);
				if (__counter == __fill)
					++__fill;
			} while (!empty());

			for (__counter = &__tmp[1]; __counter != __fill; ++__counter)
				__counter->merge(*(__counter - 1), comp);
			swap(*(__fill - 1));
		}
	}

	template<typename Type, typename Allocator>
	void list<Type, Allocator>::_transfer(const_iterator pos, const_iterator first, const_iterator last)
	{
		pos.m_node->transfer(first.m_node, last.m_node);
	}

	template<typename Type, typename Allocator>
	void list<Type, Allocator>::_default_append(size_type count)
	{
		for (; count; --count)
			emplace_back();
	}

	template<typename Type, typename Allocator>
	template<class InputIterator>
	void list<Type, Allocator>::_initialize(InputIterator first, InputIterator last)
	{
		for (; first != last; ++first)
			emplace_back(*first);
	}


}

#endif
