#ifndef LIBSTL_MOVABLE_VECTOR_HPP_
#define LIBSTL_MOVABLE_VECTOR_HPP_

#include <simstl/impl/iterator/pvt_normal_iterator.hpp>
#include <simstl/impl/iterator/reverse_iterator.hpp>
#include <simstl/impl/memory/allocator_traits.hpp>
#include <simstl/impl/memory/movable_allocator.hpp>
#include <simstl/impl/vector/base.hpp>

namespace simstd {

	template<typename Type>
	class movable_vector: public simstd::vector<Type, simstd::pvt::movable_allocator<Type> >
	{
		typedef simstd::vector<Type, simstd::pvt::movable_allocator<Type> > base_type;
		typedef movable_vector<Type> this_type;

	public:
		typedef Type value_type;
		typedef simstd::pvt::movable_allocator<Type> allocator_type;
		typedef typename base_type::size_type size_type;
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

		movable_vector() {}
		explicit movable_vector(size_type n) : base_type(n) {}
		movable_vector(size_type n, const value_type& val) : base_type(n, val) {}
		movable_vector(const this_type& other) : base_type(other) {}

		template<typename InputIterator>
		movable_vector(InputIterator first, InputIterator last) : base_type(first, last) {}

		this_type& operator =(const this_type& other);

		void lock() const;
		void unlock() const;
	};

	///=============================================================================================
	template<typename Type>
	typename
	movable_vector<Type>::this_type& movable_vector<Type>::operator =(const this_type& other)
	{
		this_type(other).swap(*this);
		return *this;
	}

	template<typename Type>
	void movable_vector<Type>::lock() const
	{
		base_type::m_impl.lock();
	}

	template<typename Type>
	void movable_vector<Type>::unlock() const
	{
		base_type::m_impl.unlock();
	}

}

#endif
