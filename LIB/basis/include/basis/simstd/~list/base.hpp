#ifndef BASIS_LIST_BASE_HPP_
#define BASIS_LIST_BASE_HPP_

#include <basis/simstd/list>

namespace simstd {

	namespace pvt {

		template<typename Type, typename Allocator>
		class List_base {
			typedef typename Allocator::template rebind<List_node<Type>>::other Node_alloc_type;
			typedef typename Allocator::template rebind<Type>::other            Type_alloc_type;

		public:
			typedef Allocator allocator_type;

			~List_base();

			List_base();

			List_base(const Node_alloc_type& __a) ;

			List_base(List_base&& __x);

			Node_alloc_type& _M_get_Node_allocator();

			const Node_alloc_type& _M_get_Node_allocator() const;

			Type_alloc_type _M_get_Tp_allocator() const;

			allocator_type get_allocator() const;

		protected:
			List_node<Type>* _M_get_node();

			void _M_put_node(List_node<Type>* __p);

			void clear();

			struct List_impl: public Node_alloc_type {
				List_impl() :
					Node_alloc_type(),
					m_node(&m_node, &m_node)
				{
				}

				List_impl(const Node_alloc_type& other) :
					Node_alloc_type(other),
					m_node(&m_node, &m_node)
				{
				}

				List_impl(Node_alloc_type&& other) :
					Node_alloc_type(simstd::move(other)),
					m_node(&m_node, &m_node)
				{
				}

				List_node_base m_node;
			};

			List_impl m_impl;
		};

		template<typename Type, typename Allocator>
		List_base<Type, Allocator>::~List_base()
		{
			clear();
		}

		template<typename Type, typename Allocator>
		List_base<Type, Allocator>::List_base() :
			m_impl()
		{
		}

		template<typename Type, typename Allocator>
		List_base<Type, Allocator>::List_base(const Node_alloc_type& __a) :
			m_impl(__a)
		{
		}

		template<typename Type, typename Allocator>
		List_base<Type, Allocator>::List_base(List_base&& __x) :
			m_impl(simstd::move(__x._M_get_Node_allocator()))
		{
			List_node_base::swap(m_impl._M_node, __x.m_impl._M_node);
		}

		template<typename Type, typename Allocator>
		typename List_base<Type, Allocator>::Node_alloc_type& List_base<Type, Allocator>::_M_get_Node_allocator()
		{
			return *static_cast<Node_alloc_type*>(&m_impl);
		}

		template<typename Type, typename Allocator>
		const typename List_base<Type, Allocator>::Node_alloc_type& List_base<Type, Allocator>::_M_get_Node_allocator() const
		{
			return *static_cast<const Node_alloc_type*>(&m_impl);
		}

		template<typename Type, typename Allocator>
		typename List_base<Type, Allocator>::Type_alloc_type List_base<Type, Allocator>::_M_get_Tp_allocator() const
		{
			return Type_alloc_type(_M_get_Node_allocator());
		}

		template<typename Type, typename Allocator>
		typename List_base<Type, Allocator>::allocator_type List_base<Type, Allocator>::get_allocator() const
		{
			return allocator_type(_M_get_Node_allocator());
		}

		template<typename Type, typename Allocator>
		List_node<Type>* List_base<Type, Allocator>::_M_get_node()
		{
			return m_impl.Node_alloc_type::allocate(1);
		}

		template<typename Type, typename Allocator>
		void List_base<Type, Allocator>::_M_put_node(List_node<Type>* __p)
		{
			m_impl.Node_alloc_type::deallocate(__p, 1);
		}

		template<typename Type, typename Allocator>
		void List_base<Type, Allocator>::clear()
		{
		}

	}

}

#endif
