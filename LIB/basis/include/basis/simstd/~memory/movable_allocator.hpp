#ifndef BASIS_MEMORY_MOVABLE_ALLOCATOR_HPP_
#define BASIS_MEMORY_MOVABLE_ALLOCATOR_HPP_

#include <basis/sys/~memory/heap/Default.hpp>

namespace simstd {

	namespace pvt {
		template<typename Type>
		struct movable_allocator {
			typedef Type value_type;
			typedef Type* pointer;
			typedef const Type* const_pointer;
			typedef Type& reference;
			typedef const Type& const_reference;
			typedef size_t size_type;
			typedef ptrdiff_t difference_type;
			typedef void* movable_handle;

			~movable_allocator();
			movable_allocator();

			movable_handle allocate(size_type cnt);
			void deallocate(movable_handle hnd);

			pointer lock(movable_handle hnd) const;
			void unlock(movable_handle hnd) const;

			void construct(pointer ptr, const_reference val);
			void destroy(pointer ptr);
		};

		template<typename Type>
		movable_allocator<Type>::~movable_allocator()
		{
		}

		template<typename Type>
		movable_allocator<Type>::movable_allocator()
		{
		}

		template<typename Type>
		typename movable_allocator<Type>::movable_handle movable_allocator<Type>::allocate(size_type cnt)
		{
			return (movable_handle)memory::heap::DefaultMovable::alloc(sizeof(Type) * cnt);
		}

		template<typename Type>
		void movable_allocator<Type>::deallocate(movable_handle hnd)
		{
			memory::heap::DefaultMovable::free(hnd);
		}

		template<typename Type>
		typename movable_allocator<Type>::pointer movable_allocator<Type>::lock(movable_handle hnd) const
		{
			return (pointer)memory::heap::DefaultMovable::lock(hnd);
		}

		template<typename Type>
		void movable_allocator<Type>::unlock(movable_handle hnd) const
		{
			memory::heap::DefaultMovable::unlock(hnd);
		}

		template<typename Type>
		void movable_allocator<Type>::construct(pointer ptr, const_reference val)
		{
			simstd::pvt::_construct(ptr, val);
		}

		template<typename Type>
		void movable_allocator<Type>::destroy(pointer ptr)
		{
			simstd::pvt::_destroy(ptr);
		}
	}

}

#endif
