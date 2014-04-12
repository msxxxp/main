#ifndef LIBSTL_MEMORY_ALLOCATOR_HPP_
#define LIBSTL_MEMORY_ALLOCATOR_HPP_

#include <system/base_types.hpp>
#include <simstd/impl/memory/base.hpp>
#include <simstd/impl/new.hpp>

namespace simstd {

	template<typename Type>
	class allocator;

	template<>
	class allocator<void>
	{
	public:
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
		typedef void* pointer;
		typedef const void* const_pointer;
		typedef void value_type;

		template<typename TypeOther>
		struct rebind
		{
			typedef allocator<TypeOther> other;
		};
	};

	template<typename Type>
	class allocator
	{
	public:
		typedef Type value_type;
		typedef Type* pointer;
		typedef const Type* const_pointer;
		typedef Type& reference;
		typedef const Type& const_reference;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;

		template<typename TypeOther>
		struct rebind
		{
			typedef allocator<TypeOther> other;
		};

		~allocator();

		allocator();
		allocator(const allocator& other);
		template<class TypeOther>
		allocator(const allocator<TypeOther>& other);

		pointer address(reference r) const;
		const_pointer address(const_reference r) const;

		pointer allocate(size_type cnt, allocator<void>::const_pointer hint = 0);
		void deallocate(pointer ptr, size_type cnt);

		size_type max_size() const;

//		void construct(pointer ptr, const_reference val);
//		void destroy(pointer ptr);

		template<typename PtrType, typename ... Args>
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
	allocator<Type>::allocator(const allocator<Type>& /*other*/)
	{
	}

	template<typename Type>
	template<class TypeOther>
	allocator<Type>::allocator(const allocator<TypeOther>& /*other*/)
	{
	}

	template<typename Type>
	typename allocator<Type>::pointer allocator<Type>::address(reference r) const
	{
		return simstd::addressof(r);
	}

	template<typename Type>
	typename allocator<Type>::const_pointer allocator<Type>::address(const_reference r) const
	{
		return simstd::addressof(r);
	}

	template<typename Type>
	typename allocator<Type>::pointer allocator<Type>::allocate(size_type cnt, allocator<void>::const_pointer /*hint*/)
	{
		return static_cast<Type*>(::operator new(sizeof(Type) * cnt, simstd::nothrow));
	}

	template<typename Type>
	void allocator<Type>::deallocate(pointer ptr, size_type /*cnt*/)
	{
		::operator delete(ptr, simstd::nothrow);
	}

	template<typename Type>
	typename allocator<Type>::size_type allocator<Type>::max_size() const
	{
		return static_cast<size_type>(-1) / sizeof(Type);
	}

//	template<typename Type>
//	void allocator<Type>::construct(pointer ptr, const_reference val)
//	{
//		simstd::pvt::_construct(ptr, val);
//	}

	template<typename Type>
	template<typename PtrType, typename ... Args>
	void allocator<Type>::construct(PtrType* ptr, Args&&... args)
	{
		::new (static_cast<void*>(ptr), simstd::nothrow) Type(simstd::forward<Args>(args)...);
	}

//	template<typename Type>
//	void allocator<Type>::destroy(pointer ptr)
//	{
//		simstd::pvt::_destroy(ptr);
//	}

	template<typename Type>
	template<typename PtrType>
	void allocator<Type>::destroy(PtrType* ptr)
	{
		UNUSED(ptr); // stupid VC
		ptr->~Type();
	}

	template<typename Type1, typename Type2>
	inline bool operator==(const allocator<Type1>&, const allocator<Type2>&)
	{
		return true;
	}

	template<typename Type>
	inline bool operator==(const allocator<Type>&, const allocator<Type>&)
	{
		return true;
	}

	template<typename Type1, typename Type2>
	inline bool operator!=(const allocator<Type1>&, const allocator<Type2>&)
	{
		return false;
	}

	template<typename Type>
	inline bool operator!=(const allocator<Type>&, const allocator<Type>&)
	{
		return false;
	}

}

#endif
