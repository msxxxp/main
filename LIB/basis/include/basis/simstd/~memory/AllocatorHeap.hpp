#ifndef BASIS_MEMORY_ALLOCATORHEAP_HPP_
#define BASIS_MEMORY_ALLOCATORHEAP_HPP_

namespace simstd {

	template<typename Type, typename HeapType>
	class AllocatorHeap {
		typedef AllocatorHeap<Type, HeapType> this_type;

	public:
		typedef Type        value_type;
		typedef Type*       pointer;
		typedef const Type* const_pointer;
		typedef Type&       reference;
		typedef const Type& const_reference;
		typedef size_t      size_type;
		typedef ptrdiff_t   difference_type;

		template<typename TypeOther>
		struct rebind {
			typedef AllocatorHeap<TypeOther, HeapType> other;
		};

		~AllocatorHeap();

		AllocatorHeap();
		AllocatorHeap(const this_type& other);

		template<class TypeOther>
		AllocatorHeap(const AllocatorHeap<TypeOther, HeapType>& other);

		pointer       address(reference r) const;
		const_pointer address(const_reference r) const;

		pointer allocate(size_type cnt, simstd::allocator<void>::const_pointer hint = 0);
		void    deallocate(pointer ptr, size_type cnt);

		size_type max_size() const;

		template<typename PtrType, typename ... Args>
		void construct(PtrType* ptr, Args&&... args);

		template<typename PtrType>
		void destroy(PtrType* ptr);
	};

	template<typename Type, typename HeapType>
	AllocatorHeap<Type, HeapType>::~AllocatorHeap()
	{
	}

	template<typename Type, typename HeapType>
	AllocatorHeap<Type, HeapType>::AllocatorHeap()
	{
	}

	template<typename Type, typename HeapType>
	AllocatorHeap<Type, HeapType>::AllocatorHeap(const this_type& /*other*/)
	{
	}

	template<typename Type, typename HeapType>
	template<class TypeOther>
	AllocatorHeap<Type, HeapType>::AllocatorHeap(const AllocatorHeap<TypeOther, HeapType>& /*other*/)
	{
	}

	template<typename Type, typename HeapType>
	typename AllocatorHeap<Type, HeapType>::pointer AllocatorHeap<Type, HeapType>::address(reference r) const
	{
		return simstd::addressof(r);
	}

	template<typename Type, typename HeapType>
	typename AllocatorHeap<Type, HeapType>::const_pointer AllocatorHeap<Type, HeapType>::address(const_reference r) const
	{
		return simstd::addressof(r);
	}

	template<typename Type, typename HeapType>
	typename AllocatorHeap<Type, HeapType>::pointer AllocatorHeap<Type, HeapType>::allocate(size_type cnt, simstd::allocator<void>::const_pointer /*hint*/)
	{
		return static_cast<pointer>(HostAlloc(HeapType, sizeof(Type) * cnt));
	}

	template<typename Type, typename HeapType>
	void AllocatorHeap<Type, HeapType>::deallocate(pointer ptr, size_type /*cnt*/)
	{
		HostFree(HeapType, ptr);
	}

	template<typename Type, typename HeapType>
	typename AllocatorHeap<Type, HeapType>::size_type AllocatorHeap<Type, HeapType>::max_size() const
	{
		return HeapType::size() / sizeof(Type);
	}

	template<typename Type, typename HeapType>
	template<typename PtrType, typename ... Args>
	void AllocatorHeap<Type, HeapType>::construct(PtrType* ptr, Args&&... args)
	{
		::new (static_cast<void*>(ptr), simstd::nothrow) Type(simstd::forward<Args>(args)...);
	}

	template<typename Type, typename HeapType>
	template<typename PtrType>
	void AllocatorHeap<Type, HeapType>::destroy(PtrType* ptr)
	{
		ptr->~Type();
	}

	template<typename Type, typename HeapType>
	inline bool operator==(const AllocatorHeap<Type, HeapType>&, const AllocatorHeap<Type, HeapType>&)
	{
		return true;
	}

	template<typename Type1, typename Type2, typename HeapType>
	inline bool operator==(const AllocatorHeap<Type1, HeapType>&, const AllocatorHeap<Type2, HeapType>&)
	{
		return true;
	}

	template<typename Type, typename HeapType>
	inline bool operator !=(const AllocatorHeap<Type, HeapType>&, const AllocatorHeap<Type, HeapType>&)
	{
		return false;
	}

	template<typename Type1, typename Type2, typename HeapType>
	inline bool operator!=(const AllocatorHeap<Type1, HeapType>&, const AllocatorHeap<Type2, HeapType>&)
	{
		return false;
	}

}

#endif
