#ifndef LIBSTL_MEMORY_HPP_
#define LIBSTL_MEMORY_HPP_

#include <simstl/types.hpp>
#include <simstl/impl/new.hpp>
#include <simstl/impl/utility/pair.hpp>

namespace simstd {

	namespace pvt {
		template<typename Type>
		Type* _addressof(Type& ref)
		{
			return reinterpret_cast<Type*>(&const_cast<char&>(reinterpret_cast<const volatile char&>(ref)));
		}

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
