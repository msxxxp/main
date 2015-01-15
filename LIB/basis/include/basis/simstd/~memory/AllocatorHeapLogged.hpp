#ifndef BASIS_MEMORY_ALLOCATORLOGGED_HPP_
#define BASIS_MEMORY_ALLOCATORLOGGED_HPP_

#include <basis/sys/console.hpp>

namespace simstd {

	template<typename Type, typename HeapType>
	class AllocatorHeapLogged {
		typedef AllocatorHeapLogged<Type, HeapType> this_type;

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
			typedef AllocatorHeapLogged<TypeOther, HeapType> other;
		};

		~AllocatorHeapLogged();

		AllocatorHeapLogged();
		AllocatorHeapLogged(const this_type& other);

		template<class TypeOther>
		AllocatorHeapLogged(const AllocatorHeapLogged<TypeOther, HeapType>& other);

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
	AllocatorHeapLogged<Type, HeapType>::~AllocatorHeapLogged()
	{
		console::printf(L"%S()\n", __PRETTY_FUNCTION__);
	}

	template<typename Type, typename HeapType>
	AllocatorHeapLogged<Type, HeapType>::AllocatorHeapLogged()
	{
		console::printf(L"%S()\n", __PRETTY_FUNCTION__);
	}

	template<typename Type, typename HeapType>
	AllocatorHeapLogged<Type, HeapType>::AllocatorHeapLogged(const this_type& /*other*/)
	{
		console::printf(L"%S()\n", __PRETTY_FUNCTION__);
	}

	template<typename Type, typename HeapType>
	template<class TypeOther>
	AllocatorHeapLogged<Type, HeapType>::AllocatorHeapLogged(const AllocatorHeapLogged<TypeOther, HeapType>& /*other*/)
	{
		console::printf(L"%S()\n", __PRETTY_FUNCTION__);
	}

	template<typename Type, typename HeapType>
	typename AllocatorHeapLogged<Type, HeapType>::pointer AllocatorHeapLogged<Type, HeapType>::address(reference r) const
	{
		this_type::pointer ret = simstd::addressof(r);
		console::printf(L"%S() -> %p\n", __PRETTY_FUNCTION__, ret);
		return ret;
	}

	template<typename Type, typename HeapType>
	typename AllocatorHeapLogged<Type, HeapType>::const_pointer AllocatorHeapLogged<Type, HeapType>::address(const_reference r) const
	{
		this_type::const_pointer ret = simstd::addressof(r);
		console::printf(L"%S() -> %p\n", __PRETTY_FUNCTION__, ret);
		return ret;
	}

	template<typename Type, typename HeapType>
	typename AllocatorHeapLogged<Type, HeapType>::pointer AllocatorHeapLogged<Type, HeapType>::allocate(size_type cnt, simstd::allocator<void>::const_pointer hint)
	{
		this_type::pointer ret = static_cast<pointer>(HostAlloc(HeapType, sizeof(Type) * cnt));
		console::printf(L"%S() [%Iu, %p, %Iu] -> %p\n", __PRETTY_FUNCTION__, cnt, hint, sizeof(Type), ret);
		return ret;
	}

	template<typename Type, typename HeapType>
	void AllocatorHeapLogged<Type, HeapType>::deallocate(pointer ptr, size_type cnt)
	{
		console::printf(L"%S() [%p, %Iu]\n", __PRETTY_FUNCTION__, ptr, cnt);
		HostFree(HeapType, ptr);
	}

	template<typename Type, typename HeapType>
	typename AllocatorHeapLogged<Type, HeapType>::size_type AllocatorHeapLogged<Type, HeapType>::max_size() const
	{
		this_type::size_type ret = HeapType::size() / sizeof(Type);
		console::printf(L"%S() -> %Iu\n", __PRETTY_FUNCTION__, ret);
		return ret;
	}

	template<typename Type, typename HeapType>
	template<typename PtrType, typename ... Args>
	void AllocatorHeapLogged<Type, HeapType>::construct(PtrType* ptr, Args&&... args)
	{
		console::printf(L"%S() [%p]\n", __PRETTY_FUNCTION__, ptr);
		::new (static_cast<void*>(ptr), simstd::nothrow) Type(simstd::forward<Args>(args)...);
	}

	template<typename Type, typename HeapType>
	template<typename PtrType>
	void AllocatorHeapLogged<Type, HeapType>::destroy(PtrType* ptr)
	{
		console::printf(L"%S() [%p]\n", __PRETTY_FUNCTION__, ptr);
		ptr->~Type();
	}

	template<typename Type, typename HeapType>
	inline bool operator==(const AllocatorHeapLogged<Type, HeapType>&, const AllocatorHeapLogged<Type, HeapType>&)
	{
		console::printf(L"%S()\n", __PRETTY_FUNCTION__);
		return true;
	}

	template<typename Type1, typename Type2, typename HeapType>
	inline bool operator==(const AllocatorHeapLogged<Type1, HeapType>&, const AllocatorHeapLogged<Type2, HeapType>&)
	{
		console::printf(L"%S()\n", __PRETTY_FUNCTION__);
		return true;
	}

	template<typename Type, typename HeapType>
	inline bool operator !=(const AllocatorHeapLogged<Type, HeapType>&, const AllocatorHeapLogged<Type, HeapType>&)
	{
		console::printf(L"%S()\n", __PRETTY_FUNCTION__);
		return false;
	}

	template<typename Type1, typename Type2, typename HeapType>
	inline bool operator!=(const AllocatorHeapLogged<Type1, HeapType>&, const AllocatorHeapLogged<Type2, HeapType>&)
	{
		console::printf(L"%S()\n", __PRETTY_FUNCTION__);
		return false;
	}

}

#endif
