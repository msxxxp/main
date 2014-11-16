#ifndef BASIS_MEMORY_HPP_
#define BASIS_MEMORY_HPP_

#include <basis/simstd/~memory/allocator.hpp>
#include <basis/simstd/~memory/allocator_traits.hpp>
#include <basis/simstd/~memory/new.hpp>
#include <basis/simstd/~utility/pair.hpp>

namespace simstd {

	namespace pvt {
//		template<typename Type>
//		Type* _addressof(Type& ref)
//		{
//			return reinterpret_cast<Type*>(&const_cast<char&>(reinterpret_cast<const volatile char&>(ref)));
//		}

		template<typename Type>
		Type* _allocate(size_t cnt)
		{
			return static_cast<Type*>(::operator new(sizeof(Type) * cnt, simstd::nothrow));
		}

		template<typename Type>
		void _deallocate(Type* ptr, size_t /*cnt*/ = 0)
		{
			::operator delete(ptr, simstd::nothrow);
		}

		template<typename Type1, typename Type2>
		void _construct(Type1* ptr, const Type2& val)
		{
			::new (static_cast<void*>(ptr), simstd::nothrow) Type1(val);
		}

		template<typename Allocator, typename ForwardIterator>
		ForwardIterator _construct_default(Allocator& allocator, ForwardIterator first, ForwardIterator last)
		{
			typedef allocator_traits<Allocator> traits_type;
			for (; first != last; ++first)
				traits_type::construct(allocator, simstd::addressof(*first));
			return first;
		}

		template<typename Allocator, typename ForwardIterator, typename Type>
		ForwardIterator _construct_copy(Allocator& allocator, ForwardIterator first, ForwardIterator last, const Type& value)
		{
			typedef allocator_traits<Allocator> traits_type;
			for (; first != last; ++first)
				traits_type::construct(allocator, simstd::addressof(*first), value);
			return first;
		}

		template<typename Type, typename... Args>
		void _construct(Type* ptr, Args&&... args)
		{
			::new (static_cast<void*>(ptr), simstd::nothrow) Type(simstd::forward<Args>(args)...);
		}

		template<typename Type>
		void _destroy(Type* ptr)
		{
			ptr->~Type();
		}

		template<typename Type, typename ForwardIterator>
		void _destroy(ForwardIterator first, ForwardIterator last)
		{
			for (; first != last; ++first)
				first->~Type();
		}

		template<typename Allocator, typename ForwardIterator>
		void _destroy(Allocator& allocator, ForwardIterator first, ForwardIterator last)
		{
			typedef allocator_traits<Allocator> traits_type;
			for (; first != last; ++first)
				traits_type::destroy(allocator, simstd::addressof(*first));
		}

	}

//	template<bool>
//	struct _Destroy_aux {
//		template<typename ForwardIterator>
//		static void _destroy(ForwardIterator first, ForwardIterator last)
//		{
//			for (; first != last; ++first)
//				simstd::_destroy(simstd::_addressof(*first));
//		}
//	};
//
//	template<>
//	struct _Destroy_aux<true> {
//		template<typename ForwardIterator>
//		static void _destroy(ForwardIterator, ForwardIterator)
//		{
//		}
//	};
//
//	template<typename ForwardIterator>
//	void _destroy(ForwardIterator first, ForwardIterator last)
//	{
//		typedef typename iterator_traits<ForwardIterator>::value_type Value_type;
//		simstd::_Destroy_aux<__has_trivial_destructor(Value_type)>::_destroy(first, last);
//	}
//
//	template<typename ForwardIterator, typename Allocator>
//	void _destroy(ForwardIterator first, ForwardIterator last, Allocator& alloc)
//	{
//		for (; first != last; ++first)
//			alloc.destroy(simstd::_addressof(*first));
//	}
//
//	template<typename ForwardIterator, typename Type>
//	void _destroy(ForwardIterator first, ForwardIterator last, allocator<Type>&)
//	{
//		_destroy(first, last);
//	}

	template<typename Type>
	simstd::pair<Type*, ptrdiff_t> get_temporary_buffer(ptrdiff_t cnt)
	{
		Type* ptr = static_cast<Type*>(::operator new(sizeof(Type) * cnt, simstd::nothrow));
		return simstd::pair<Type*, ptrdiff_t>(ptr, ptr ? cnt : 0);
	}

	template<typename Type>
	void return_temporary_buffer(Type* ptr)
	{
		::operator delete(ptr, simstd::nothrow);
	}

}

#endif
