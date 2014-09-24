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

		protected:
			Node_alloc_type& get_node_allocator();

			const Node_alloc_type& get_node_allocator() const;

			Type_alloc_type get_type_allocator() const;

//			allocator_type get_allocator() const;

			template<typename ... Args>
			List_node<Type>* create_node(Args &&... args);

			void delete_node(const List_node_base * ptr);

			List_node<Type>* alloc_node();

			void free_node(List_node<Type>* ptr);

			void clear();

			struct List_impl: public Node_alloc_type {
				List_impl();
				List_impl(const Node_alloc_type& other);
				List_impl(Node_alloc_type&& other);
				List_node_base m_end;
			};

			List_impl m_impl;
		};

		template<typename Type, typename Allocator>
		List_base<Type, Allocator>::List_impl::List_impl() :
			Node_alloc_type()
		{
			LogTraceObj();
		}

		template<typename Type, typename Allocator>
		List_base<Type, Allocator>::List_impl::List_impl(const Node_alloc_type& other) :
			Node_alloc_type(other)
		{
			LogTraceObj();
		}

		template<typename Type, typename Allocator>
		List_base<Type, Allocator>::List_impl::List_impl(Node_alloc_type&& other) :
			Node_alloc_type(simstd::move(other))
		{
			LogTraceObj();
		}

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
			m_impl(simstd::move(other.get_node_allocator()))
		{
			LogTraceObj();
			LogDebug(L"&m_impl: %p\n", &m_impl.m_end);
			LogDebug(L"&m_prev: %p\n", m_impl.m_end.m_next);
			LogDebug(L"&m_next: %p\n", m_impl.m_end.m_prev);
			List_node_base::swap(m_impl.m_end, other.m_impl.m_end);
		}

		template<typename Type, typename Allocator>
		typename List_base<Type, Allocator>::Node_alloc_type& List_base<Type, Allocator>::get_node_allocator()
		{
			return *static_cast<Node_alloc_type*>(&m_impl);
		}

		template<typename Type, typename Allocator>
		const typename List_base<Type, Allocator>::Node_alloc_type& List_base<Type, Allocator>::get_node_allocator() const
		{
			return *static_cast<const Node_alloc_type*>(&m_impl);
		}

		template<typename Type, typename Allocator>
		typename List_base<Type, Allocator>::Type_alloc_type List_base<Type, Allocator>::get_type_allocator() const
		{
			return Type_alloc_type(get_node_allocator());
		}

//		template<typename Type, typename Allocator>
//		typename List_base<Type, Allocator>::allocator_type List_base<Type, Allocator>::get_allocator() const
//		{
//			return allocator_type(get_node_allocator());
//		}

		template<typename Type, typename Allocator>
		template<typename ... Args>
		List_node<Type>* List_base<Type, Allocator>::create_node(Args &&... args)
		{
			List_node<Type>* ptr = alloc_node();
			get_node_allocator().construct(ptr, simstd::forward<Args>(args)...);
			return ptr;
		}

		template<typename Type, typename Allocator>
		void List_base<Type, Allocator>::delete_node(const List_node_base * ptr)
		{
			List_node<Type>* node = static_cast<List_node<Type>*>(const_cast<List_node_base*>(ptr));
			get_node_allocator().destroy(node);
			free_node(node);
		}

		template<typename Type, typename Allocator>
		List_node<Type>* List_base<Type, Allocator>::alloc_node()
		{
			return m_impl.Node_alloc_type::allocate(1);
		}

		template<typename Type, typename Allocator>
		void List_base<Type, Allocator>::free_node(List_node<Type>* p)
		{
			m_impl.Node_alloc_type::deallocate(p, 1);
		}

		template<typename Type, typename Allocator>
		void List_base<Type, Allocator>::clear()
		{
		}

	}

}

#endif
