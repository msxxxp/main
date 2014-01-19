#ifndef LIBSTL_MEMORY_ALLOCATOR_HPP_
#define LIBSTL_MEMORY_ALLOCATOR_HPP_

#include <simstl/types.hpp>

namespace simstd {

	template<typename Type>
	struct allocator {
		typedef Type value_type;
		typedef Type* pointer;
		typedef const Type* const_pointer;
		typedef Type& reference;
		typedef const Type& const_reference;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;

		~allocator();
		allocator();

		pointer allocate(size_type cnt);
		void deallocate(pointer ptr, size_type cnt);

		void construct(pointer ptr, const_reference val);
		void destroy(pointer ptr);
	};

	template<typename Type>
	allocator<Type>::~allocator()
	{
	}

	template<typename Type>
	allocator<Type>::allocator()
	{
	}

	template<typename Type>
	typename allocator<Type>::pointer allocator<Type>::allocate(size_type cnt)
	{
		return simstd::pvt::_allocate<Type>(cnt);
	}

	template<typename Type>
	void allocator<Type>::deallocate(pointer ptr, size_type cnt)
	{
		simstd::pvt::_deallocate(ptr, cnt);
	}

	template<typename Type>
	void allocator<Type>::construct(pointer ptr, const_reference val)
	{
		simstd::pvt::_construct(ptr, val);
	}

	template<typename Type>
	void allocator<Type>::destroy(pointer ptr)
	{
		simstd::pvt::_destroy(ptr);
	}

}

#endif
