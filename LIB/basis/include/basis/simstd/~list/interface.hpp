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
		void     remove_if(UnaryPredicate p);

		void     reverse();

		void     unique();
		template<class BinaryPredicate>
		void     unique(BinaryPredicate p);

		void     sort();
		template<class Compare>
		void     sort(Compare comp);

	protected:
//		typedef simstd::pvt::vector_base<value_type, allocator_type> impl_type;
//		impl_type m_impl;

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
		for (; count; --count)
			emplace_back();
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
		base_type(other._M_get_Node_allocator())
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
		return iterator(base_type::m_impl.m_node.m_next);
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
		return const_iterator(base_type::m_impl.m_node.m_next);
	}

	template<typename Type, typename Allocator>
	typename
	list<Type, Allocator>::iterator list<Type, Allocator>::end()
	{
		return iterator(&base_type::m_impl.m_node);
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
		return const_iterator(&base_type::m_impl.m_node);
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
		return base_type::m_impl.m_node.m_next == &base_type::m_impl.m_node;
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
		return base_type::_M_get_Node_allocator().max_size();
	}

	template<typename Type, typename Allocator>
	void list<Type, Allocator>::clear()
	{
		base_type::clear();
		base_type::init();
	}

	template<typename Type, typename Allocator>
	template<typename InputIterator>
	typename
	list<Type, Allocator>::iterator list<Type, Allocator>::insert(const_iterator pos, InputIterator first, InputIterator last)
	{
		this_type tmp(first, last, get_allocator());
		splice(pos, tmp);
	}

	template<typename Type, typename Allocator>
	typename
	list<Type, Allocator>::iterator list<Type, Allocator>::insert(const_iterator pos, const value_type& value)
	{
		return emplace(pos, value);
	}

	template<typename Type, typename Allocator>
	typename
	list<Type, Allocator>::iterator list<Type, Allocator>::insert(const_iterator pos, size_type n, const value_type& value)
	{
		this_type tmp(n, value, get_allocator());
		splice(pos, tmp);
	}

	template<typename Type, typename Allocator>
	typename
	list<Type, Allocator>::iterator list<Type, Allocator>::insert(const_iterator pos, value_type&& value)
	{
		return emplace(pos, simstd::move(value));
	}

	template<typename Type, typename Allocator>
	template<typename... Args>
	typename
	list<Type, Allocator>::iterator list<Type, Allocator>::emplace(const_iterator pos, Args&&... args)
	{
		typedef typename simstd::pvt::List_node<Type> Node;
		Node* tmp = base_type::create_node(simstd::forward<Args>(args)...);
		tmp->hook(pos.m_node);
		return iterator(tmp);
	}

	template<typename Type, typename Allocator>
	typename
	list<Type, Allocator>::iterator list<Type, Allocator>::erase(const_iterator pos)
	{
		pos.m_node->unhook();

		typedef typename simstd::pvt::List_node<Type> Node;
		Node* ptr = static_cast<Node*>(pos.m_node);
		base_type::_M_get_Node_allocator().destroy(ptr);
		base_type::_M_put_node(ptr);
	}

	template<typename Type, typename Allocator>
	typename
	list<Type, Allocator>::iterator list<Type, Allocator>::erase(const_iterator first, const_iterator last)
	{
		while (first != last)
			first = erase(first);
		return last._M_const_cast();
	}

	template<typename Type, typename Allocator>
	void list<Type, Allocator>::push_back(const value_type& value)
	{
		emplace(cend(), value);
	}

	template<typename Type, typename Allocator>
	void list<Type, Allocator>::push_back(value_type&& value)
	{
		emplace(cend(), simstd::move(value));
	}

	template<typename Type, typename Allocator>
	template<typename... Args>
	void list<Type, Allocator>::emplace_back(Args&&... args)
	{
		emplace(cend(), simstd::forward<Args>(args)...);
	}

	template<typename Type, typename Allocator>
	void list<Type, Allocator>::pop_back()
	{
		erase(cend()--);
	}

	template<typename Type, typename Allocator>
	void list<Type, Allocator>::push_front(const value_type& value)
	{
		emplace(cbegin(), value);
	}

	template<typename Type, typename Allocator>
	void list<Type, Allocator>::push_front(value_type&& value)
	{
		emplace(cbegin(), simstd::move(value));
	}

	template<typename Type, typename Allocator>
	template<typename... Args>
	void list<Type, Allocator>::emplace_front(Args&&... args)
	{
		emplace(cbegin(), simstd::forward<Args>(args)...);
	}

	template<typename Type, typename Allocator>
	void list<Type, Allocator>::pop_front()
	{
		erase(cbegin());
	}

//	template<typename Type, typename Allocator>
//	void list<Type, Allocator>::resize(size_type count)
//	{
//	}
//
//	template<typename Type, typename Allocator>
//	void list<Type, Allocator>::resize(size_type count, const value_type& value)
//	{
//	}

















	template<typename Type, typename Allocator>
	template<class InputIterator>
	void list<Type, Allocator>::_initialize(InputIterator first, InputIterator last)
	{
		for (; first != last; ++first)
			emplace_back(*first);
	}


}

#endif
