#ifndef SARALIB_STL_VECTOR_IMPL_HPP_
#define SARALIB_STL_VECTOR_IMPL_HPP_

#include "impl_types.hpp"
#include "impl_memory.hpp"

namespace sarastd {
	template<typename Type, typename Allocator>
	struct _vector_impl: private Allocator {
		typedef _vector_impl this_type;
		typedef Type value_type;
		typedef Allocator allocator_type;
		typedef typename allocator_traits<allocator_type>::size_type size_type;
		typedef typename allocator_traits<allocator_type>::pointer pointer;

		pointer begin;
		pointer end;
		pointer end_of_storage;

		~_vector_impl();
		_vector_impl();
		_vector_impl(size_type capa);
		_vector_impl(size_type capa, const value_type& value);
		_vector_impl(size_type capa, pointer first, pointer last);
		void swap(this_type& other);
		void destroy(pointer first, pointer last);
		void reserve(size_type newCapacity);
		void adjust_capacity(size_type add);
		bool check_capacity(size_type add) const;
		size_type get_new_capacity(size_type add) const;
		size_type capacity() const;
		size_type size() const;
		size_type max_size() const;

	private:
		void create_storage(size_type capa);
	};

	template<typename Type, typename Allocator>
	_vector_impl<Type, Allocator>::~_vector_impl()
	{
		printf("%s\n", __PRETTY_FUNCTION__);
		if (begin)
			allocator_type::deallocate(begin, capacity());
	}

	template<typename Type, typename Allocator>
	_vector_impl<Type, Allocator>::_vector_impl() :
		begin(0), end(0), end_of_storage(0)
	{
		printf("%s\n", __PRETTY_FUNCTION__);
	}

	template<typename Type, typename Allocator>
	_vector_impl<Type, Allocator>::_vector_impl(size_type capa) :
		begin(0), end(0), end_of_storage(0)
	{
		create_storage(capa);
	}

	template<typename Type, typename Allocator>
	_vector_impl<Type, Allocator>::_vector_impl(size_type capa, const value_type& value) :
		begin(0), end(0), end_of_storage(0)
	{
		create_storage(capa);
		sarastd::uninitialized_fill_n(begin, capa, value);
		end += capa;
	}

	template<typename Type, typename Allocator>
	_vector_impl<Type, Allocator>::_vector_impl(size_type capa, pointer first, pointer last) :
		begin(0), end(0), end_of_storage(0)
	{
		create_storage(capa);
		sarastd::uninitialized_copy(first, last, end);
		end += (last - first);
	}

	template<typename Type, typename Allocator>
	void _vector_impl<Type, Allocator>::swap(this_type& other)
	{
		using sarastd::swap;
		swap(begin, other.begin);
		swap(end, other.end);
		swap(end_of_storage, other.end_of_storage);
	}

	template<typename Type, typename Allocator>
	void _vector_impl<Type, Allocator>::create_storage(size_type capa)
	{
		end = begin = (capa) ? allocator_type::allocate(capa) : 0;
		end_of_storage = begin + (begin ? capa : 0);
	}

	template<typename Type, typename Allocator>
	void _vector_impl<Type, Allocator>::destroy(pointer first, pointer last)
	{
		for (; first != last; ++first)
			allocator_type::destroy(first);
	}

	template<typename Type, typename Allocator>
	void _vector_impl<Type, Allocator>::reserve(size_type newCapacity)
	{
		if (capacity() < newCapacity)
			this_type(newCapacity, begin, end).swap(*this);
	}

	template<typename Type, typename Allocator>
	typename
	_vector_impl<Type, Allocator>::size_type _vector_impl<Type, Allocator>::size() const
	{
		return end - begin;
	}

	template<typename Type, typename Allocator>
	typename
	_vector_impl<Type, Allocator>::size_type _vector_impl<Type, Allocator>::max_size() const
	{
		return 0xFFFFFFFFu;
	}

	template<typename Type, typename Allocator>
	typename
	_vector_impl<Type, Allocator>::size_type _vector_impl<Type, Allocator>::capacity() const
	{
		return end_of_storage - begin;
	}

	template<typename Type, typename Allocator>
	bool _vector_impl<Type, Allocator>::check_capacity(size_type add) const
	{
		if ((size() + add) > capacity())
			return false;
		return true;
	}

	template<typename Type, typename Allocator>
	typename
	_vector_impl<Type, Allocator>::size_type _vector_impl<Type, Allocator>::get_new_capacity(size_type add) const
	{
		return size() + sarastd::max(size_type(4), sarastd::max(size(), add));
	}

	template<typename Type, typename Allocator>
	void _vector_impl<Type, Allocator>::adjust_capacity(size_type add)
	{
		if (!check_capacity(add))
			reserve(get_new_capacity(add));
	}

	///=============================================================================================
	template<typename Type>
	struct _vector_impl<Type, movable_allocator<Type> >: private movable_allocator<Type>  {
		typedef _vector_impl<Type, movable_allocator<Type> > this_type;
		typedef Type value_type;
		typedef movable_allocator<Type> allocator_type;
		typedef typename allocator_traits<allocator_type>::size_type size_type;
		typedef typename allocator_traits<allocator_type>::pointer pointer;
		typedef typename allocator_type::movable_handle movable_handle;

		mutable pointer begin;
		mutable pointer end;
		mutable pointer end_of_storage;

		~_vector_impl();
		_vector_impl();
		_vector_impl(size_type capa);
		_vector_impl(size_type capa, const value_type& value);
		_vector_impl(size_type capa, pointer first, pointer last);
		void swap(this_type& other);
		void destroy(pointer first, pointer last);
		void reserve(size_type newCapacity);
		void adjust_capacity(size_type add);
		bool check_capacity(size_type add) const;
		size_type get_new_capacity(size_type add) const;
		size_type capacity() const;
		size_type size() const;
		size_type max_size() const;

		void lock() const;
		void unlock() const;

	private:
		void create_storage(size_type capa);

		movable_handle handle;
	};

	template<typename Type>
	_vector_impl<Type, movable_allocator<Type> >::~_vector_impl()
	{
		printf("%s\n", __PRETTY_FUNCTION__);
		if (handle) {
			lock();
			while (end != begin)
				allocator_type::destroy(--end);
			unlock();
			allocator_type::deallocate(handle);
		}
	}

	template<typename Type>
	_vector_impl<Type, movable_allocator<Type> >::_vector_impl() :
		begin(0), end(0), end_of_storage(0), handle(0)
	{
		printf("%s\n", __PRETTY_FUNCTION__);
	}

	template<typename Type>
	_vector_impl<Type, movable_allocator<Type> >::_vector_impl(size_type capa) :
		begin(0), end(0), end_of_storage(0), handle(0)
	{
		create_storage(capa);
	}

	template<typename Type>
	_vector_impl<Type, movable_allocator<Type> >::_vector_impl(size_type capa, const value_type& value) :
		begin(0), end(0), end_of_storage(0), handle(0)
	{
		create_storage(capa);
		lock();
		sarastd::uninitialized_fill_n(begin, capa, value);
		end += capa;
		unlock();
	}

	template<typename Type>
	_vector_impl<Type, movable_allocator<Type> >::_vector_impl(size_type capa, pointer first, pointer last) :
		begin(0), end(0), end_of_storage(0), handle(0)
	{
		create_storage(capa);
		lock();
		sarastd::uninitialized_copy(first, last, end);
		end += (last - first);
		unlock();
	}

	template<typename Type>
	void _vector_impl<Type, movable_allocator<Type> >::swap(this_type& other)
	{
		using sarastd::swap;
		swap(begin, other.begin);
		swap(end, other.end);
		swap(end_of_storage, other.end_of_storage);
		swap(handle, other.handle);
	}

	template<typename Type>
	void _vector_impl<Type, movable_allocator<Type> >::create_storage(size_type capa)
	{
		handle = (capa) ? allocator_type::allocate(capa) : 0;
		end = begin = 0;
		end_of_storage = begin + (handle ? capa : 0);
	}

	template<typename Type>
	void _vector_impl<Type, movable_allocator<Type> >::destroy(pointer first, pointer last)
	{
		for (; first != last; ++first)
			allocator_type::destroy(first);
	}

	template<typename Type>
	void _vector_impl<Type, movable_allocator<Type> >::reserve(size_type newCapacity)
	{
		if (capacity() < newCapacity) {
			lock();
			this_type tmp(newCapacity, begin, end);
			unlock();
			tmp.swap(*this);
		}
	}

	template<typename Type>
	typename
	_vector_impl<Type, movable_allocator<Type> >::size_type _vector_impl<Type, movable_allocator<Type> >::size() const
	{
		return end - begin;
	}

	template<typename Type>
	typename
	_vector_impl<Type, movable_allocator<Type> >::size_type _vector_impl<Type, movable_allocator<Type> >::max_size() const
	{
		return 0xFFFFFFFFu;
	}

	template<typename Type>
	typename
	_vector_impl<Type, movable_allocator<Type> >::size_type _vector_impl<Type, movable_allocator<Type> >::capacity() const
	{
		return end_of_storage - begin;
	}

	template<typename Type>
	bool _vector_impl<Type, movable_allocator<Type> >::check_capacity(size_type add) const
	{
		if ((size() + add) > capacity())
			return false;
		return true;
	}

	template<typename Type>
	typename
	_vector_impl<Type, movable_allocator<Type> >::size_type _vector_impl<Type, movable_allocator<Type> >::get_new_capacity(size_type add) const
	{
		return size() + sarastd::max(size_type(4), sarastd::max(size(), add));
	}

	template<typename Type>
	void _vector_impl<Type, movable_allocator<Type> >::adjust_capacity(size_type add)
	{
		if (!check_capacity(add))
			reserve(get_new_capacity(add));
	}

	template<typename Type>
	void _vector_impl<Type, movable_allocator<Type> >::lock() const
	{
		printf("%s\n", __PRETTY_FUNCTION__);
		pointer oldBegin = begin;
		begin = (pointer)allocator_type::lock(handle);
		sarastd::ptrdiff_t diff = begin - oldBegin;
		end += diff;
		end_of_storage += diff;
	}

	template<typename Type>
	void _vector_impl<Type, movable_allocator<Type> >::unlock() const
	{
		printf("%s\n", __PRETTY_FUNCTION__);
		sarastd::ptrdiff_t diff = begin - pointer(0);
		end_of_storage -= diff;
		end -= diff;
		begin = 0;
		allocator_type::unlock(handle);
	}

}

#endif
