#ifndef BASIS_MEMORY_ALLOCATOR_EQ_HPP_
#define BASIS_MEMORY_ALLOCATOR_EQ_HPP_

#include <basis/types.hpp>
#include <basis/sys/memory.hpp>
#include <basis/sys/console.hpp>
#include <basis/simstd/~memory/allocator.hpp>

namespace simstd {

	template<typename Type>
	class AllocatorEq {
	public:
		typedef Type value_type;
		typedef Type* pointer;
		typedef const Type* const_pointer;
		typedef Type& reference;
		typedef const Type& const_reference;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;

		template<typename TypeOther>
		struct rebind {
			typedef AllocatorEq<TypeOther> other;
		};

		~AllocatorEq();

		AllocatorEq();
		AllocatorEq(const AllocatorEq& other);
		template<class TypeOther>
		AllocatorEq(const AllocatorEq<TypeOther>& other);

		pointer address(reference r) const;
		const_pointer address(const_reference r) const;

		pointer allocate(size_type cnt, simstd::allocator<void>::const_pointer hint = 0);
		void deallocate(pointer ptr, size_type cnt);

		size_type max_size() const;

		template<typename PtrType, typename ... Args>
		void construct(PtrType* ptr, Args&&... args);

		template<typename PtrType>
		void destroy(PtrType* ptr);
	};

	template<typename Type>
	AllocatorEq<Type>::~AllocatorEq()
	{
		console::printf(L"%S()\n", __FUNCTION__);
	}

	template<typename Type>
	AllocatorEq<Type>::AllocatorEq()
	{
		console::printf(L"%S()\n", __FUNCTION__);
	}

	template<typename Type>
	AllocatorEq<Type>::AllocatorEq(const AllocatorEq<Type>& /*other*/)
	{
		console::printf(L"%S()\n", __FUNCTION__);
	}

	template<typename Type>
	template<class TypeOther>
	AllocatorEq<Type>::AllocatorEq(const AllocatorEq<TypeOther>& /*other*/)
	{
		console::printf(L"%S()\n", __FUNCTION__);
	}

	template<typename Type>
	typename AllocatorEq<Type>::pointer AllocatorEq<Type>::address(reference r) const
	{
		console::printf(L"%S()\n", __FUNCTION__);
		return simstd::addressof(r);
	}

	template<typename Type>
	typename AllocatorEq<Type>::const_pointer AllocatorEq<Type>::address(const_reference r) const
	{
		console::printf(L"%S()\n", __FUNCTION__);
		return simstd::addressof(r);
	}

	template<typename Type>
	typename AllocatorEq<Type>::pointer AllocatorEq<Type>::allocate(size_type cnt, simstd::allocator<void>::const_pointer /*hint*/)
	{
		console::printf(L"%S()\n", __FUNCTION__);
		return static_cast<pointer>(memory::HeapSpecial1::alloc(sizeof(Type) * cnt));
	}

	template<typename Type>
	void AllocatorEq<Type>::deallocate(pointer ptr, size_type /*cnt*/)
	{
		console::printf(L"%S()\n", __FUNCTION__);
		memory::HeapSpecial1::free(ptr);
	}

	template<typename Type>
	typename AllocatorEq<Type>::size_type AllocatorEq<Type>::max_size() const
	{
		console::printf(L"%S()\n", __FUNCTION__);
		return memory::HeapSpecial1::size() / sizeof(Type);
	}

	template<typename Type>
	template<typename PtrType, typename ... Args>
	void AllocatorEq<Type>::construct(PtrType* ptr, Args&&... args)
	{
		console::printf(L"%S()\n", __FUNCTION__);
		::new (static_cast<void*>(ptr), simstd::nothrow) Type(simstd::forward<Args>(args)...);
	}

	template<typename Type>
	template<typename PtrType>
	void AllocatorEq<Type>::destroy(PtrType* ptr)
	{
		console::printf(L"%S()\n", __FUNCTION__);
		UNUSED(ptr); // stupid VC
		ptr->~Type();
	}

	template<typename Type1, typename Type2>
	inline bool operator==(const AllocatorEq<Type1>&, const AllocatorEq<Type2>&)
	{
		return true;
	}

	template<typename Type>
	inline bool operator==(const AllocatorEq<Type>&, const AllocatorEq<Type>&)
	{
		return true;
	}

	template<typename Type1, typename Type2>
	inline bool operator!=(const AllocatorEq<Type1>&, const AllocatorEq<Type2>&)
	{
		return false;
	}

	template<typename Type>
	inline bool operator !=(const AllocatorEq<Type>&, const AllocatorEq<Type>&)
	{
		return false;
	}

}

#endif
