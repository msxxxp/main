#ifndef BASIS_LIST_ITERATOR_HPP_
#define BASIS_LIST_ITERATOR_HPP_

#include <basis/simstd/list>

namespace simstd {

	namespace pvt {

		template<typename Type>
		struct List_iterator {
			typedef List_iterator<Type> this_type;
			typedef List_node<Type>     node_type;

			typedef simstd::bidirectional_iterator_tag iterator_category;
			typedef ptrdiff_t           difference_type;
			typedef Type                value_type;
			typedef Type*               pointer;
			typedef Type&               reference;

			List_iterator() noexcept : m_node() {}

			explicit List_iterator(List_node_base * node) noexcept : m_node(node) {}

			this_type _M_const_cast() const noexcept {return *this;}

			reference operator *() const noexcept {return static_cast<node_type*>(m_node)->m_data;}

			pointer operator ->() const noexcept {return simstd::addressof(static_cast<node_type*>(m_node)->m_data);}

			this_type& operator ++() noexcept {m_node = m_node->m_next; return *this;}

			this_type operator ++(int) noexcept {this_type tmp = *this; m_node = m_node->m_next; return tmp;}

			this_type& operator --() noexcept {m_node = m_node->m_prev; return *this;}

			this_type operator --(int) noexcept {this_type tmp = *this; m_node = m_node->m_prev; return tmp;}

			bool operator ==(const this_type & other) const noexcept {return m_node == other.m_node;}

			bool operator !=(const this_type & other) const noexcept {return m_node != other.m_node;}

			List_node_base* m_node;
		};

		template<typename Type>
		struct List_const_iterator {
			typedef List_const_iterator<Type> this_type;
			typedef const List_node<Type>     node_type;
			typedef List_iterator<Type>       iterator;

			typedef simstd::bidirectional_iterator_tag iterator_category;
			typedef ptrdiff_t                 difference_type;
			typedef Type                      value_type;
			typedef const Type*               pointer;
			typedef const Type&               reference;

			List_const_iterator() noexcept : m_node() {}

			explicit List_const_iterator(const List_node_base* node) noexcept : m_node(node) {}

			List_const_iterator(const iterator& it) noexcept : m_node(it.m_node) {}

			iterator _M_const_cast() const noexcept {return iterator(const_cast<List_node_base*>(m_node));}

			reference operator*() const noexcept {return static_cast<node_type*>(m_node)->m_data;}

			pointer operator->() const noexcept {return simstd::addressof(static_cast<node_type*>(m_node)->m_data);}

			this_type& operator++() noexcept {m_node = m_node->m_next; return *this;}

			this_type operator++(int) noexcept {this_type tmp = *this; m_node = m_node->m_next; return tmp;}

			this_type& operator--() noexcept {m_node = m_node->m_prev; return *this;}

			this_type operator--(int) noexcept {this_type tmp = *this; m_node = m_node->m_prev; return tmp;}

			bool operator ==(const this_type & other) const noexcept {return m_node == other.m_node;}

			bool operator !=(const this_type & other) const noexcept {return m_node != other.m_node;}

			const List_node_base* m_node;
		};

		template<typename Value>
		bool operator ==(const List_iterator<Value> & a, const List_const_iterator<Value> & b) noexcept
		{
			return a.m_node == b.m_node;
		}

		template<typename Value>
		bool operator !=(const List_iterator<Value> & a, const List_const_iterator<Value> & b) noexcept
		{
			return a.m_node != b.m_node;
		}

	}

}

#endif
