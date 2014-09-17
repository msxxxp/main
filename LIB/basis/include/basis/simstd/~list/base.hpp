#ifndef BASIS_LIST_BASE_HPP_
#define BASIS_LIST_BASE_HPP_

#include <basis/simstd/list>

namespace simstd {

	namespace pvt {

		template<typename Type, typename Allocator>
		class List_base {
		protected:
			typedef typename Allocator::template rebind<List_node<Type>>::other Node_alloc_type;
			typedef typename Allocator::template rebind<Type>::other            Type_alloc_type;

		public:
			typedef Allocator allocator_type;

			~List_base();

			List_base();

			List_base(const Node_alloc_type& __a) ;

			List_base(List_base && other);

			Node_alloc_type& _M_get_Node_allocator();

			const Node_alloc_type& _M_get_Node_allocator() const;

			Type_alloc_type _M_get_Tp_allocator() const;

			allocator_type get_allocator() const;

			template<typename ... Args>
			List_node<Type>* create_node(Args &&... args);

		protected:
			List_node<Type>* _M_get_node();

			void _M_put_node(List_node<Type>* __p);

			void clear();

			struct List_impl: public Node_alloc_type {
				List_impl() :
					Node_alloc_type(),
					m_node(&m_node, &m_node)
				{
					LogTraceObj();
				}

				List_impl(const Node_alloc_type& other) :
					Node_alloc_type(other),
					m_node(&m_node, &m_node)
				{
					LogTraceObj();
				}

				List_impl(Node_alloc_type&& other) :
					Node_alloc_type(simstd::move(other)),
					m_node(&m_node, &m_node)
				{
					LogTraceObj();
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
		List_base<Type, Allocator>::List_base(const Node_alloc_type& allocator) :
			m_impl(allocator)
		{
		}

		template<typename Type, typename Allocator>
		List_base<Type, Allocator>::List_base(List_base && other) :
			m_impl(simstd::move(other._M_get_Node_allocator()))
		{
			LogTraceObj();
			LogDebug(L"&m_impl: %p\n", &m_impl.m_node);
			LogDebug(L"&m_prev: %p\n", m_impl.m_node.m_next);
			LogDebug(L"&m_next: %p\n", m_impl.m_node.m_prev);
			List_node_base::swap(m_impl.m_node, other.m_impl.m_node);
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
		template<typename ... Args>
		List_node<Type>* List_base<Type, Allocator>::create_node(Args &&... args)
		{
			List_node<Type>* ptr = _M_get_node();
			_M_get_Node_allocator().construct(ptr, simstd::forward<Args>(args)...);
			return ptr;
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
