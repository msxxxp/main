#ifndef BASIS_LIST_BASE_HPP_
#define BASIS_LIST_BASE_HPP_

#include <basis/simstd/list>

namespace simstd {

	namespace pvt {

		template<typename Type, typename TypeAllocator>
		class List_base {
		protected:
			typedef typename TypeAllocator::template rebind<List_node<Type>>::other NodeAllocator;

		public:
			typedef TypeAllocator allocator_type;

			List_base();

			List_base(const NodeAllocator& allocator) ;

			List_base(List_base && other);

		protected:
			NodeAllocator& get_node_allocator();

			const NodeAllocator& get_node_allocator() const;

			template<typename ... Args>
			List_node<Type>* new_node(Args &&... args);

			void delete_node(const List_node_base * ptr);

			struct List_impl: public NodeAllocator {
				List_impl();
				List_impl(const NodeAllocator& other);
				List_impl(NodeAllocator&& other);
				List_node_base m_end;
			};

			List_impl m_impl;

		private:
			List_node<Type>* alloc_node();

			void free_node(List_node<Type>* ptr);
		};

		template<typename Type, typename Allocator>
		List_base<Type, Allocator>::List_impl::List_impl() :
			NodeAllocator()
		{
		}

		template<typename Type, typename Allocator>
		List_base<Type, Allocator>::List_impl::List_impl(const NodeAllocator& other) :
			NodeAllocator(other)
		{
		}

		template<typename Type, typename Allocator>
		List_base<Type, Allocator>::List_impl::List_impl(NodeAllocator&& other) :
			NodeAllocator(simstd::move(other))
		{
		}

		template<typename Type, typename Allocator>
		List_base<Type, Allocator>::List_base() :
			m_impl()
		{
		}

		template<typename Type, typename Allocator>
		List_base<Type, Allocator>::List_base(const NodeAllocator& allocator) :
			m_impl(allocator)
		{
		}

		template<typename Type, typename Allocator>
		List_base<Type, Allocator>::List_base(List_base && other) :
			m_impl(simstd::move(other.get_node_allocator()))
		{
			List_node_base::swap(m_impl.m_end, other.m_impl.m_end);
		}

		template<typename Type, typename Allocator>
		typename List_base<Type, Allocator>::NodeAllocator& List_base<Type, Allocator>::get_node_allocator()
		{
			return *static_cast<NodeAllocator*>(&m_impl);
		}

		template<typename Type, typename Allocator>
		const typename List_base<Type, Allocator>::NodeAllocator& List_base<Type, Allocator>::get_node_allocator() const
		{
			return *static_cast<const NodeAllocator*>(&m_impl);
		}

		template<typename Type, typename Allocator>
		template<typename ... Args>
		List_node<Type>* List_base<Type, Allocator>::new_node(Args &&... args)
		{
			auto ptr = alloc_node();
			get_node_allocator().construct(ptr, simstd::forward<Args>(args)...);
			return ptr;
		}

		template<typename Type, typename Allocator>
		void List_base<Type, Allocator>::delete_node(const List_node_base * ptr)
		{
			auto node = static_cast<List_node<Type>*>(const_cast<List_node_base*>(ptr));
			get_node_allocator().destroy(node);
			free_node(node);
		}

		template<typename Type, typename Allocator>
		List_node<Type>* List_base<Type, Allocator>::alloc_node()
		{
			return m_impl.NodeAllocator::allocate(1);
		}

		template<typename Type, typename Allocator>
		void List_base<Type, Allocator>::free_node(List_node<Type>* p)
		{
			m_impl.NodeAllocator::deallocate(p, 1);
		}

	}

}

#endif
