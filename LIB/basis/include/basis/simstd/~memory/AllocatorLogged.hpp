#ifndef BASIS_MEMORY_ALLOCATORLOGGED_HPP_
#define BASIS_MEMORY_ALLOCATORLOGGED_HPP_

#include <basis/types.hpp>
#include <basis/sys/console.hpp>

namespace simstd {

	template<typename Type, typename HeapType>
	class AllocatorLogged {
		typedef AllocatorLogged<Type, HeapType> this_type;

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
			typedef AllocatorLogged<TypeOther, HeapType> other;
		};

		~AllocatorLogged();

		AllocatorLogged();
		AllocatorLogged(const this_type& other);

		template<class TypeOther>
		AllocatorLogged(const AllocatorLogged<TypeOther, HeapType>& other);

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
	AllocatorLogged<Type, HeapType>::~AllocatorLogged()
	{
		console::printf(L"%S()\n", __PRETTY_FUNCTION__);
	}

	template<typename Type, typename HeapType>
	AllocatorLogged<Type, HeapType>::AllocatorLogged()
	{
		console::printf(L"%S()\n", __PRETTY_FUNCTION__);
	}

	template<typename Type, typename HeapType>
	AllocatorLogged<Type, HeapType>::AllocatorLogged(const this_type& /*other*/)
	{
		console::printf(L"%S()\n", __PRETTY_FUNCTION__);
	}

	template<typename Type, typename HeapType>
	template<class TypeOther>
	AllocatorLogged<Type, HeapType>::AllocatorLogged(const AllocatorLogged<TypeOther, HeapType>& /*other*/)
	{
		console::printf(L"%S()\n", __PRETTY_FUNCTION__);
	}

	template<typename Type, typename HeapType>
	typename AllocatorLogged<Type, HeapType>::pointer AllocatorLogged<Type, HeapType>::address(reference r) const
	{
		this_type::pointer ret = simstd::addressof(r);
		console::printf(L"%S() -> %p\n", __PRETTY_FUNCTION__, ret);
		return ret;
	}

	template<typename Type, typename HeapType>
	typename AllocatorLogged<Type, HeapType>::const_pointer AllocatorLogged<Type, HeapType>::address(const_reference r) const
	{
		this_type::const_pointer ret = simstd::addressof(r);
		console::printf(L"%S() -> %p\n", __PRETTY_FUNCTION__, ret);
		return ret;
	}

	template<typename Type, typename HeapType>
	typename AllocatorLogged<Type, HeapType>::pointer AllocatorLogged<Type, HeapType>::allocate(size_type cnt, simstd::allocator<void>::const_pointer hint)
	{
		this_type::pointer ret = static_cast<pointer>(HeapType::alloc(sizeof(Type) * cnt));
		console::printf(L"%S() [%Iu, %p, %Iu] -> %p\n", __PRETTY_FUNCTION__, cnt, hint, sizeof(Type), ret);
		return ret;
	}

	template<typename Type, typename HeapType>
	void AllocatorLogged<Type, HeapType>::deallocate(pointer ptr, size_type cnt)
	{
		console::printf(L"%S() [%p, %Iu]\n", __PRETTY_FUNCTION__, ptr, cnt);
		HeapType::free(ptr);
	}

	template<typename Type, typename HeapType>
	typename AllocatorLogged<Type, HeapType>::size_type AllocatorLogged<Type, HeapType>::max_size() const
	{
		this_type::size_type ret = HeapType::size() / sizeof(Type);
		console::printf(L"%S() -> %Iu\n", __PRETTY_FUNCTION__, ret);
		return ret;
	}

	template<typename Type, typename HeapType>
	template<typename PtrType, typename ... Args>
	void AllocatorLogged<Type, HeapType>::construct(PtrType* ptr, Args&&... args)
	{
		console::printf(L"%S() [%p]\n", __PRETTY_FUNCTION__, ptr);
		::new (static_cast<void*>(ptr), simstd::nothrow) Type(simstd::forward<Args>(args)...);
	}

	template<typename Type, typename HeapType>
	template<typename PtrType>
	void AllocatorLogged<Type, HeapType>::destroy(PtrType* ptr)
	{
		console::printf(L"%S() [%p]\n", __PRETTY_FUNCTION__, ptr);
		ptr->~Type();
	}

	template<typename Type, typename HeapType>
	inline bool operator==(const AllocatorLogged<Type, HeapType>&, const AllocatorLogged<Type, HeapType>&)
	{
		console::printf(L"%S()\n", __PRETTY_FUNCTION__);
		return true;
	}

	template<typename Type1, typename Type2, typename HeapType>
	inline bool operator==(const AllocatorLogged<Type1, HeapType>&, const AllocatorLogged<Type2, HeapType>&)
	{
		console::printf(L"%S()\n", __PRETTY_FUNCTION__);
		return true;
	}

	template<typename Type, typename HeapType>
	inline bool operator !=(const AllocatorLogged<Type, HeapType>&, const AllocatorLogged<Type, HeapType>&)
	{
		console::printf(L"%S()\n", __PRETTY_FUNCTION__);
		return false;
	}

	template<typename Type1, typename Type2, typename HeapType>
	inline bool operator!=(const AllocatorLogged<Type1, HeapType>&, const AllocatorLogged<Type2, HeapType>&)
	{
		console::printf(L"%S()\n", __PRETTY_FUNCTION__);
		return false;
	}

}

#endif
