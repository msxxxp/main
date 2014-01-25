#ifndef LIBSTL_MEMORY_ALLOCATOR_HPP_
#define LIBSTL_MEMORY_ALLOCATOR_HPP_

#include <simstd/types.hpp>
#include <simstd/impl/memory/base.hpp>

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

		template<typename PtrType, typename... Args>
		void construct(PtrType* ptr, Args&&... args);

		template<typename PtrType>
		void destroy(PtrType* ptr);
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
	template<typename PtrType, typename... Args>
	void allocator<Type>::construct(PtrType* ptr, Args&&... args)
	{
		simstd::pvt::_construct(ptr, simstd::forward<Args>(args)...);
	}

	template<typename Type>
	void allocator<Type>::destroy(pointer ptr)
	{
		simstd::pvt::_destroy(ptr);
	}

	template<typename Type>
	template<typename PtrType>
	void allocator<Type>::destroy(PtrType* ptr)
	{
		simstd::pvt::_destroy(ptr);
	}
}

#endif
